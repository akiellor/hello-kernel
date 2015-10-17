CC=i686-elf-gcc
CCFLAGS=-c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
AS=i686-elf-as
ASFLAGS=

OUTDIR=target
MKDIR_P=mkdir -p

directories: ${OUTDIR}

${OUTDIR}:
	${MKDIR_P} ${OUTDIR}


${OUTDIR}/myos.bin: ${OUTDIR}/boot.o ${OUTDIR}/kernel.o
	$(CC) -T src/linker.ld -o $@ -ffreestanding -O2 -nostdlib -lgcc $?

program: ${OUTDIR}/myos.bin
	mkdir -p target/isodir/boot/grub
	cp target/myos.bin target/isodir/boot/myos.bin
	cp src/grub.cfg target/isodir/boot/grub/grub.cfg
	grub-mkrescue -o target/myos.iso target/isodir

${OUTDIR}/%.o: src/%.s
	$(AS) $(ASFLAGS) $< -o $@

${OUTDIR}/%.o: src/%.c
	$(CC) $< -o $@ $(CCFLAGS)

clean:
	rm -rf target

all: directories program

run:
	qemu-system-i386 -serial file:${OUTDIR}/serial.out -curses ${OUTDIR}/myos.iso
