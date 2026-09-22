### How to use MCUmgr:

Install go tool:

```shell
brew install go
```

# Updating MCU over UART/USB-CDC
Firstly, go must be installed. After that, mcumgr and scripting-tools
can be installed via following command:
```shell
go install github.com/apache/mynewt-mcumgr-cli/mcumgr@latest
go install github.com/arduino/scripting-tools@latest
```

mcumgr path can be find via following command:
```shell
go env GOPATH
```

To test, send a string to the remote target device and have it echo it back:
```shell
mcumgr version
mcumgr --conntype serial --connstring "/dev/cu.usbserial-xxx,baud=115200" echo hello
```

Other commands to update mcu:
```shell
mcumgr --conntype serial --connstring <connection string> image upload -n 1 ../firmwares/zephyr-rak4631_nrf52840.bin
mcumgr --conntype serial --connstring <connection string> image upload -n 2 ../sketch/arduino_blinky/build/rak.zephyr.rak4631/arduino_blinky.ino.bin-zsk.bin

mcumgr --conntype serial --connstring <connection string> image list
mcumgr --conntype serial --connstring <connection string> image confirm <hash of slot-1 image>
mcumgr --conntype serial --connstring <connection string> reset
```
