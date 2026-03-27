CC = gcc
AS = nasm
# Adicionamos -m32 para compilar em 32 bits no seu sistema 64 bits
LDFLAGS = -m32 -ffreestanding -O2 -nostdlib -lgcc -T linker.ld
CFLAGS = -m32 -c -ffreestanding -O2 -Wall -Wextra

all: bbin/tasma-kernel.bin

bbin/boot.o: tcore/boot.asm
	$(AS) -felf32 tcore/boot.asm -o bbin/boot.o

bbin/kernel.o: tcore/kernel.c
	$(CC) $(CFLAGS) tcore/kernel.c -o bbin/kernel.o

bbin/tvga.o: tasdev/tvga.c
	$(CC) $(CFLAGS) tasdev/tvga.c -o bbin/tvga.o

bbin/tasma-kernel.bin: bbin/boot.o bbin/kernel.o bbin/tvga.o
	$(CC) $(LDFLAGS) -o bbin/tasma-kernel.bin bbin/boot.o bbin/kernel.o bbin/tvga.o

clean:
	rm -rf bbin/*.o bbin/*.bin

run: all
	qemu-system-i386 -kernel bbin/tasma-kernel.bin
