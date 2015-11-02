CC=i686-elf-gcc
CCFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
AS=nasm
ASFLAGS=-felf32

OUTDIR=target
MKDIR_P=mkdir -p

.PHONY: directories

directories: ${OUTDIR}

${OUTDIR}:
	${MKDIR_P} ${OUTDIR}

${OUTDIR}/myos.bin: ${OUTDIR}/boot.o ${OUTDIR}/kernel.o ${OUTDIR}/io.o ${OUTDIR}/serial.o
	$(CC) -T src/linker.ld -o $@ -ffreestanding -O2 -nostdlib -lgcc $?

program: ${OUTDIR}/myos.bin
	mkdir -p ${OUTDIR}/isodir/boot/grub
	cp ${OUTDIR}/myos.bin ${OUTDIR}/isodir/boot/myos.bin
	cp src/grub.cfg ${OUTDIR}/isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${OUTDIR}/myos.iso ${OUTDIR}/isodir

${OUTDIR}/%.o: src/%.asm
	$(AS) $(ASFLAGS) $< -o $@

${OUTDIR}/%.o: src/%.c
	$(CC) $< -o $@ $(CCFLAGS)

clean:
	rm -rf target

all: directories program

run:
  qemu-system-i386 -serial file:${OUTDIR}/serial.out -curses ${OUTDIR}/myos.iso
