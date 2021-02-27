#delete target files if there is an error or make is interrupted
.DELETE_ON_ERROR:

# target = nanOS.img
objdir = ./obj
bindir = ./bin
bootblock = $(bindir)/bootblock
bootblockobj = $(objdir)/bootblock.o
all:$(target)

$(target): $(bootblock)

$(bootblock):$(bootblockobj)
	gcc -o $(bootblock) $(bootblockobj)
$(bootblockobj):
