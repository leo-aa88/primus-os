<img src="print_screen.png"/>

# PrimusOS

A simple bare-bones 32-bit Ring 0 operating system written in Assembly x86 and C for educational purposes. <br>
Inspired by Terry A. Davis (in memoriam).

## Dependencies for building

`xorriso` <br>
`mtools` <br>

## Bootloader "hello world" example

### Compile bootloader

`nasm -o bootloader.bin bootloader.asm`

### Testing with QEmu

`qemu-system-x86_64 bootloader.bin`

## Compiling the kernel example

`gcc -c kernel.c -o kernel -ffreestanding -m32`

## Generating the ISO image

`make check_dir`
`make clean` <br>
`make primus-os.iso`

## About pull requests

PLEASE, comment on the related issue BEFORE sending a pull request! Thanks. Contributions and improvements are always welcome.

## TODO

- Implement unit tests and bechmarks (math functions) <br>
- Implement memory management <br>
- Add multithreading <br>
- Implement calculator (parsing of command-line tokens) <br>
- Implement basic text editor <br>
- Create documentation <br>
- Add games (tic-tac-toe, chess, etc.) <br>
- Implement other VGA graphic modes <br>
- Implement complex number library in C++ <br>
- Implement linear algebra library, scientific computing, etc. <br>
