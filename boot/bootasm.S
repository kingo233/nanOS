#include <asm.h>
.set PROT_MODE_CSEG,        0x8                     # kernel code segment selector
.set PROT_MODE_DSEG,        0x10                    # kernel data segment selector
.set CR0_PE_ON,             0x1                     # protected mode enable flag

# Bootstrap GDT
.p2align 2                                          # force 4 byte alignment
gdt:
    SEG_NULLASM                                     # null seg
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)           # code seg for bootloader and kernel
    SEG_ASM(STA_W, 0x0, 0xffffffff)                 # data seg for bootloader and kernel

gdtdesc:
    .word 0x17                                      # sizeof(gdt) - 1
    .long gdt                                       # address gdt
#real mode code
.globl start
start:
    .code16         #code for 16-bit mode
    cli             #disable interrupts
    cld             #string operations increment


    #set up important data segment registers(DS,ES,SS)
    movw $0x0, %ax     #mow,w stand for 16 bit
    movw %ax, %ds
    movw %ax,%es
    movw %ax,%ss
    
    #Eable A20

    seta20.1:
        inb $0x64, %al                                  # Wait for not busy(8042 input buffer empty).
        testb $0x2, %al
        jnz seta20.1

        movb $0xd1, %al                                 # 0xd1 -> port 0x64
        outb %al, $0x64                                 # 0xd1 means: write data to 8042's P2 port

    seta20.2:
        inb $0x64, %al                                  # Wait for not busy(8042 input buffer empty).
        testb $0x2, %al
        jnz seta20.2

        movb $0xdf, %al                                 # 0xdf -> port 0x60
        outb %al, $0x60                                 # 0xdf = 11011111, means set P2's A20 bit(the 1 bit) to 1
        
        # Switch from real to protected mode, using a bootstrap GDT
        # and segment translation that makes virtual addresses
        # identical to physical addresses, so that the
        # effective memory map does not change during the switch.
        lgdt gdtdesc                                    # load gdt's base address and length to GDTR
        movl %cr0, %eax                                 # get the value of CR0
        orl $CR0_PE_ON, %eax                            # first bit to 1
        movl %eax, %cr0                                 # change the value of CR0

        # Jump to next instruction, but in 32-bit code segment.
        # Switches processor into 32-bit mode.
        ljmp $PROT_MODE_CSEG, $protcseg                 # clean the command flow of the CPU because we have changed mode of CPU.


    .code32
    protcseg:
        movw $PROT_MODE_DSEG,%ax
        movw %ax,%ds
        movw %ax,%es
        movw %ax,%ss
        movw %ax,%fs
        movw %ax,%gs

        # Set up the stack pointer and call into C. The stack region is from 0--start(0x7c00)
        movl $0,%ebp
        movl $start,%esp
        call bootmain
        spin:
            jmp spin




