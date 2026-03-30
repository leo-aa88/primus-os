CC=gcc
AS=as
GCCPARAMS = -m32 -nostdlib -fno-builtin -fno-exceptions -ffreestanding -fno-leading-underscore -Wall -Wextra -Wpedantic
ASPARAMS = --32
LDPARAMS = -melf_i386

SRC_DIR=src
HDR_DIR=include/
OBJ_DIR=obj
ISO_DIR=iso

SRC_FILES1=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES1=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES1))
SRC_FILES2=$(wildcard $(SRC_DIR)/*.s)
OBJ_FILES2=$(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC_FILES2))
SRC_FILES3=$(wildcard $(SRC_DIR)/*.asm)
OBJ_FILES3=$(patsubst $(SRC_DIR)/%.asm, $(OBJ_DIR)/%.o, $(SRC_FILES3))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(GCCPARAMS) $^ -I$(HDR_DIR) -c -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) $(ASPARAMS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	nasm -f elf32 -o $@ $<

primus-os.bin: $(SRC_DIR)/linker.ld $(OBJ_FILES1) $(OBJ_FILES2) $(OBJ_FILES3)
	ld $(LDPARAMS) -T $< -o $@ $(OBJ_DIR)/*.o

primus-os.iso: primus-os.bin
	./update_version
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp primus-os.bin iso/boot/primus-os.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "PrimusOS" {'            >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/primus-os.bin'   >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=primus-os.iso iso
	rm -rf iso

install: primus-os.bin
	sudo cp $< /boot/primus-os.bin

create-disk:
	dd if=/dev/zero of=disk.img bs=1M count=64
	mkfs.fat -F32 disk.img
	echo 'drive c: file="disk.img"' > ~/.mtoolsrc
	echo "" | mcopy - c:/HISTORY.TXT

check_dir:
	if [ ! -d "$(OBJ_DIR)" ]; then \
		mkdir -p $(OBJ_DIR); \
	fi

run-qemu:
	qemu-system-i386 -kernel primus-os.bin -serial stdio -drive file=disk.img,format=raw

clean:
	rm -rf *.o primus-os primus-os.iso primus-os.bin $(OBJ_DIR)/*.o iso
