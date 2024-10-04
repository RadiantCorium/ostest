# Project RADIANCE
idk i might write smth here at some point lol

## Building
It is recommended to build this project on a Linux environment. Other operating systems are untested.

1. Create a [GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
2. Install other required packages:
    - **make** (To run the Makefile)
    - **xorriso** (To create the ISO file)
    - **grub-common** (to set up the GRUB bootloader)
    - **grub-pc-bin** (to set up the GRUB bootloader)
    - **qemu** (to test the OS, other VMs work but the makefile is designed to launch QEMU)
2. Run ``make`` in the root directors of the project (Alternatively run ``make qemu`` to build and then load the project in qemu.)