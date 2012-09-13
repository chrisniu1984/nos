CC = gcc
ASM = nasm
LD = ld

CFLAGS = -Wall -c -m32 -I./ -nostdinc -fno-stack-protector
LDFLAGS = -m elf_i386 -nostdlilb -Ttext 0
LIBS = head.o main.o vga.o idt.o

kernel: system
	objcopy -O binary -S -R .note -R .comment system kernel

system: $(LIBS)
	$(LD) $(LDFLAGS) $(^) -o $(@)

head.o: head.s
	$(ASM) -f elf -o $(@) $(<)

.c.o:
	$(CC) $(CFLAGS) -o $(*).o $(<)

clean:
	rm -f kernel system
	rm -f $(LIBS)
