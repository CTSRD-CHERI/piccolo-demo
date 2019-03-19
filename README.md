# Government Furnished Equipment (GFE)

This is a Hello World application for GFE P1 Processor (Bluespec Piccolo) synthesized on the Xilinx VCU118 Evaluation Kit.

### Build under Linux/FreeBSD

Build 32-bit toolchain: https://github.com/riscv/riscv-gnu-toolchain

    $ export CROSS_COMPILE=/path/to/gfe/riscv-tools/bin/riscv32-unknown-elf-
    $ git clone --recursive https://github.com/CTSRD-CHERI/piccolo-demo
    $ cd piccolo-demo
    $ make

### Connect to UART
    $ cu -l /dev/ttyUSB1 -s 115200

### Program the VCU118 Evaluation Board:

    $ /path/to/openocd -f /path/to/gfe/testing/targets/p1_hs2.cfg \
      -c 'halt; load_image /path/to/piccolo-demo.bin 0x80000000; \
            reg pc 0x80000000; resume; exit'

![alt text](https://raw.githubusercontent.com/CTSRD-CHERI/piccolo-demo/master/images/vcu118.jpg)
