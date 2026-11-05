// Copyright (c) 2026 RAKwireless Technology Limited
// Author: sercan.erat@rakwireless.com
// SPDX-License-Identifier: Apache-2.0

package main

import (
	"bufio"
	"bytes"
	"crypto/sha256"
	"debug/elf"
	"encoding/binary"
	"flag"
	"fmt"
	"os"
	"strings"
)

// patch is one symbol table st_value to overwrite with a SLID
type patch struct {
	offset int64
	value  []byte
}

func main() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage: %s [-config <loader .config>] <sketch.elf>\n\n", os.Args[0])
		fmt.Fprintf(os.Stderr, "Stores the SLID of every symbol the sketch imports from the loader in\n")
		fmt.Fprintf(os.Stderr, "its symbol table, like Zephyr's scripts/build/llext_inject_slids.py.\n")
		fmt.Fprintf(os.Stderr, "\nWith -config, nothing is done unless the loader was built with\n")
		fmt.Fprintf(os.Stderr, "CONFIG_LLEXT_EXPORT_BUILTINS_BY_SLID=y.\n")
	}
	config := flag.String("config", "", "loader .config file")
	flag.Parse()
	if flag.NArg() != 1 {
		flag.Usage()
		os.Exit(1)
	}

	if *config != "" {
		enabled, err := slidEnabled(*config)
		if err != nil {
			fmt.Fprintf(os.Stderr, "Error: %v\n", err)
			os.Exit(1)
		}
		if !enabled {
			return
		}
	}

	n, err := injectSlids(flag.Arg(0))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: %v\n", err)
		os.Exit(1)
	}
	if n > 0 {
		fmt.Printf("Injected %d SLIDs\n", n)
	}
}

func slidEnabled(path string) (bool, error) {
	f, err := os.Open(path)
	if err != nil {
		return false, err
	}
	defer f.Close()

	s := bufio.NewScanner(f)
	for s.Scan() {
		if strings.TrimSpace(s.Text()) == "CONFIG_LLEXT_EXPORT_BUILTINS_BY_SLID=y" {
			return true, nil
		}
	}
	return false, s.Err()
}

// SLID: the first pointer-size bytes of the SHA-256 of the symbol name, read
// big-endian (scripts/build/llext_slidlib.py)
func slid(name string, size int, order binary.ByteOrder) []byte {
	sum := sha256.Sum256([]byte(name))
	value := make([]byte, size)
	if size == 4 {
		order.PutUint32(value, binary.BigEndian.Uint32(sum[:4]))
	} else {
		order.PutUint64(value, binary.BigEndian.Uint64(sum[:8]))
	}
	return value
}

func findPatches(path string) ([]patch, error) {
	f, err := elf.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var symtab *elf.Section
	var syms []elf.Symbol
	switch f.Type {
	case elf.ET_REL:
		symtab = f.Section(".symtab")
		syms, err = f.Symbols()
	case elf.ET_DYN:
		symtab = f.Section(".dynsym")
		syms, err = f.DynamicSymbols()
	default:
		// a statically linked sketch has no imports
		return nil, nil
	}
	if symtab == nil {
		return nil, fmt.Errorf("%s: no symbol table", path)
	}
	if err != nil {
		return nil, err
	}

	size, valueOffset := 4, int64(4)
	if f.Class == elf.ELFCLASS64 {
		size, valueOffset = 8, 8
	}

	var patches []patch
	for i, s := range syms {
		bind := elf.ST_BIND(s.Info)
		if elf.ST_TYPE(s.Info) != elf.STT_NOTYPE || s.Section != elf.SHN_UNDEF ||
			(bind != elf.STB_GLOBAL && bind != elf.STB_WEAK) {
			continue
		}
		// Symbols() skips the null symbol at index 0
		offset := int64(symtab.Offset) + int64(i+1)*int64(symtab.Entsize) + valueOffset
		patches = append(patches, patch{offset, slid(s.Name, size, f.ByteOrder)})
	}
	return patches, nil
}

func injectSlids(path string) (int, error) {
	patches, err := findPatches(path)
	if err != nil || len(patches) == 0 {
		return 0, err
	}

	out, err := os.OpenFile(path, os.O_RDWR, 0)
	if err != nil {
		return 0, err
	}
	defer out.Close()

	for _, p := range patches {
		old := make([]byte, len(p.value))
		if _, err := out.ReadAt(old, p.offset); err != nil {
			return 0, err
		}
		if !bytes.Equal(old, make([]byte, len(old))) && !bytes.Equal(old, p.value) {
			return 0, fmt.Errorf("%s: unexpected non-zero st_value at offset %#x", path, p.offset)
		}
		if _, err := out.WriteAt(p.value, p.offset); err != nil {
			return 0, err
		}
	}
	return len(patches), out.Close()
}
