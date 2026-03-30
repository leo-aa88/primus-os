# PrimusOS

A minimal 32-bit x86 operating system written in C and Assembly, featuring a custom shell, FAT32 filesystem, ATA disk driver, and basic runtime (heap, math, crypto).

Built from scratch for learning low-level systems programming.

![PrimusOS screenshot](print_screen.png)

---

## Features

- FAT32 filesystem driver with ATA PIO disk I/O (read/write)
- Persistent shell history stored on disk (FAT32-backed)
- Interactive shell with command history navigation (↑)
- Multiboot-compliant kernel (GRUB / QEMU `-kernel`)
- VGA text mode terminal with color support
- PS/2 keyboard driver (capslock, shift, arrow keys)
- Heap memory allocator
- Mathematical functions (sin, cos, tan, sqrt, pow, log, etc.)
- SHA-224 and SHA-256 cryptographic hashing
- RPN calculator
- Easter eggs

---

## Dependencies

### Arch Linux

```bash
sudo pacman -S gcc nasm qemu-system-x86 mtools xorriso
```

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 mtools xorriso
```

### Fedora

```bash
sudo dnf install gcc nasm qemu-system-x86 mtools xorriso make
```

---

## Building

```bash
make primus-os.bin
```

---

## Running

```bash
# Create disk image (first time only)
make create-disk

# Run in QEMU
make run-qemu
```

---

## Generating a bootable ISO

```bash
make primus-os.iso
```

---

## Shell commands

| Command                 | Description                 |
| ----------------------- | --------------------------- |
| `help`                  | List all commands           |
| `about`                 | About PrimusOS              |
| `math`                  | List mathematical functions |
| `crypto`                | List cryptography utilities |
| `history`               | Show command history        |
| `clear`                 | Clear the screen            |
| `datetime`              | Show current date and time  |
| `date`                  | Show current date           |
| `clock`                 | Show current time           |
| `fontcolor`             | Change terminal font color  |
| `reboot`                | Reboot the system           |
| `shutdown`              | Shutdown the system         |
| `sha256(x)`             | SHA-256 hash of x           |
| `sha224(x)`             | SHA-224 hash of x           |
| `sin(x)`, `cos(x)`, ... | Math functions              |

---

## Architecture

High-level overview of kernel subsystems:

```
src/
├── loader.s        # Multiboot entry point
├── kernel.c        # Shell loop, command dispatcher
├── tty.c           # VGA terminal
├── io.c            # Port I/O, keyboard
├── ata.c           # ATA PIO disk driver
├── fat32.c         # FAT32 filesystem (read/write)
├── memory.c        # Heap allocator
├── string.c        # String utilities
├── math.c          # Math functions
├── sha224.c        # SHA-224
├── sha256.c        # SHA-256
├── calculator.c    # RPN calculator
└── shell_history.c # Persistent command history
```

---

## Roadmap

### Near term

- [ ] Filesystem commands (`ls`, `cat`, `write`)
- [ ] Full FAT32 write support (cluster allocation)

### Medium term

- [ ] Basic text editor
- [ ] Multithreading

### Long term

- [ ] VGA graphics modes
- [ ] Games (tic-tac-toe, chess)

---

## Contributing

Please open an issue before submitting a pull request.

---

## License

GNU General Public License v3.0
