## LLEXT SLID injector

With `CONFIG_LLEXT_EXPORT_BUILTINS_BY_SLID=y`, the loader exports its
symbols by Symbol Link Identifier (SLID) instead of by name, and the
names no longer take flash. A sketch must then carry, for every symbol
it imports from the loader, the SLID of that symbol in its ELF symbol
table.

This tool does that step for Arduino sketch builds, like Zephyr's
`scripts/build/llext_inject_slids.py` does for Zephyr-built extensions:
a SLID is the first pointer-size bytes of the SHA-256 of the symbol
name, read big-endian. It runs after the sketch link
(`recipe.hooks.linking.postlink.2` in `platform.txt`).

### Usage

```shell
llext-slid [-config <loader .config>] <sketch.elf>
```

With `-config`, the ELF is left untouched unless the loader `.config`
sets `CONFIG_LLEXT_EXPORT_BUILTINS_BY_SLID=y`. Statically linked
sketches have no imports and are left untouched too.

### Building manually

```shell
go build
```
