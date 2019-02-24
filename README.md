# Government Furnished Equipment (GFE)

This is a Hello World application for GFE P1 Processor (Bluespec Piccolo).

### Build under Linux/FreeBSD

Build 32-bit toolchain: https://github.com/riscv/riscv-gnu-toolchain

    $ export CROSS_COMPILE=/path/to/riscv32-unknown-linux-gnu-
    $ git clone --recursive https://github.com/CTSRD-CHERI/piccolo_demo
    $ cd piccolo_demo
    $ make

### Program to VCU118 Evaluation Board:

    $ /path/to/openocd -f /path/to/gfe/testing/targets/p1_hs2.cfg \
      -c 'halt; load_image /path/to/piccolo_demo.bin 0x80000000; \
            reg pc 0x80000000; resume; exit'
