CC = gcc
AS = nasm
LDFLAGS = -m32 -ffreestanding -O2 -nostdlib -lgcc -T linker.ld
CFLAGS = -m32 -c -ffreestanding -O2 -Wall -Wextra

OBJS = bbin/boot.o bbin/gdt_flush.o bbin/gdt.o bbin/kernel.o bbin/tvga.o

all: bbin/tasma-kernel.bin

bbin/boot.o: tcore/boot.asm
	$(AS) -felf32 tcore/boot.asm -o bbin/boot.o

bbin/gdt_flush.o: tcore/gdt_flush.asm
	$(AS) -felf32 tcore/gdt_flush.asm -o bbin/gdt_flush.o

bbin/gdt.o: tcore/gdt.c
	$(CC) $(CFLAGS) tcore/gdt.c -o bbin/gdt.o

bbin/kernel.o: tcore/kernel.c
	$(CC) $(CFLAGS) tcore/kernel.c -o bbin/kernel.o

bbin/tvga.o: tasdev/tvga.c
	$(CC) $(CFLAGS) tasdev/tvga.c -o bbin/tvga.o

bbin/tasma-kernel.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o bbin/tasma-kernel.bin $(OBJS)

clean:
	rm -rf bbin/*.o bbin/*.bin

run: all
	qemu-system-i386 -kernel bbin/tasma-kernel.bin
