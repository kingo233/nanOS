#delete target files if there is an error or make is interrupted
.DELETE_ON_ERROR:

target = bin/nanOS.img
objdir = obj
bindir = bin
bootblock = $(bindir)/bootblock
bootblockobj = $(objdir)/bootblock.o
CFLAGS := -march=i686 -fno-builtin -fno-PIC -Wall -ggdb -m32 -gstabs -nostdinc -fno-stack-protector -Os 
signtool = bin/sign
includefiles = libs
CFLAGS += -I$(includefiles)
# 上面的意思依次为指定编译的目标架构，禁用内置函数，允许使用引用绝对地址的代码，打开编译器警
# 告信息，生成32位可执行文件，生成stab格式的调试信息，不使用C中的默认库，启用栈溢出保护机制
all:
	bash init.sh
$(target): $(bootblock)
	dd if=/dev/zero of=$@ count=10000
	dd if=$(bootblock) of=$@ conv=notrunc
$(bootblock):$(bootblockobj) $(signtool)
	objcopy -S -O binary $(bootblockobj) $(objdir)/bootblock.out 
	./bin/sign $(objdir)/bootblock.out $(bootblock)
$(bootblockobj):boot/*
	gcc $(CFLAGS) -Iboot -c boot/bootasm.S -o $(objdir)/bootasm.o
	gcc $(CFLAGS) -Iboot -c boot/bootmain.c -o $(objdir)/bootmain.o
	ld -m    elf_i386 -nostdlib -N -e start -Ttext 0x7C00 obj/bootasm.o obj/bootmain.o -o obj/bootblock.o
$(signtool): tools/sign.c 
	gcc -o bin/sign $^
.PHONY: clean debug

clean:
	rm -rf obj bin

debug: all
	qemu-system-i386 -S -s -parallel stdio -hda $(target) -serial null &
	gdb -q -tui -x tools/gdbinit 
