/*************************************************************************
	> File Name: asm.h
	> Author: jiangnan
	> Mail: kingo233@outlook.com
	> Created Time: 2021年02月27日 星期六 15时12分23秒
 ************************************************************************/

#ifndef _ASM_H
#define _ASM_H
/* Assembler macros to create x86 segments */
//一个段描述符长64bit,2 * 16 + 4 * 8 = 64,就是空段描述符了
/* Normal segment */
#define SEG_NULLASM                                             \
    .word 0, 0;                                                 \
    .byte 0, 0, 0, 0

#define SEG_ASM(type,base,lim)                                  \
    .word (((lim) >> 12) & 0xffff), ((base) & 0xffff);          \
    .byte (((base) >> 16) & 0xff), (0x90 | (type)),             \
        (0xC0 | (((lim) >> 28) & 0xf)), (((base) >> 24) & 0xff)

//段描述符的属性部分，因为属性部分不止一个bit，所以用二进制的各个位来描述
/* Application segment type bits */
#define STA_X       0x8     // Executable segment
#define STA_E       0x4     // Expand down (non-executable segments)
#define STA_C       0x4     // Conforming code segment (executable only)
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)
#define STA_A       0x1     // Accessed
#endif
