run: image
	make -C bochs

image: clean boot setup kernel
	dd if=boot/boot of=bochs/a.img bs=512 count=1 conv=notrunc seek=0
	dd if=setup/setup of=bochs/a.img bs=512 count=5 conv=notrunc seek=1
	dd if=kernel/kernel of=bochs/a.img bs=512 count=12 conv=notrunc seek=6

boot:
	make -C boot

setup:
	make -C setup

kernel:
	make -C kernel

clean:
	make -C bochs clean
	make -C boot clean
	make -C setup clean
	make -C kernel clean

.PHONY:	run boot setup kernel image clean
