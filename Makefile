CC = x86_64-elf-gcc
CFLAGS = -g -fno-pic               \
	-mno-sse                       \
	-mno-sse2                      \
	-mno-mmx                       \
	-mno-80387                     \
	-mno-red-zone                  \
	-mcmodel=kernel                \
	-ffreestanding                 \
	-fno-stack-protector           \
	-O2                            \
	-fno-omit-frame-pointer        \
	-I src
LD = x86_64-elf-ld
LDFLAGS = -no-pie -nostdlib -T src/linker.ld

build:
	nasm -f elf64 src/boot.asm -o src/boot.o
	nasm -f elf64 src/sys/isr.asm -o src/sys/isr.asm.o

	$(CC) $(CFLAGS) -c src/kernel.c -o src/kernel.o
	$(CC) $(CFLAGS) -c src/devices/video/vga.c -o src/devices/video/vga.o
	$(CC) $(CFLAGS) -c src/sys/idt.c -o src/sys/idt.o
	$(CC) $(CFLAGS) -c src/sys/isr.c -o src/sys/isr.o
	$(CC) $(CFLAGS) -c src/devices/video/vbe.c -o src/devices/video/vbe.o
	$(CC) $(CFLAGS) -c src/sys/pic.c -o src/sys/pic.o
	$(LD) src/kernel.o src/boot.o src/sys/pic.o src/sys/isr.o src/sys/isr.asm.o src/devices/video/vga.o src/devices/video/vbe.o src/sys/idt.o $(LDFLAGS) -o kernel.elf

	dd if=/dev/zero bs=1M count=0 seek=64 of=os.img
	parted -s os.img mklabel msdos
	parted -s os.img mkpart primary 1 100%
	echfs-utils -m -p0 os.img quick-format 32768
	echfs-utils -m -p0 os.img import limine.cfg boot/limine.cfg
	echfs-utils -m -p0 os.img import picture.bmp boot/picture.bmp
	echfs-utils -m -p0 os.img import kernel.elf boot/kernel.elf

	./limine-install limine.bin os.img

clean:
	rm *.img src/*.o *.elf

run:
	qemu-system-x86_64 -enable-kvm os.img