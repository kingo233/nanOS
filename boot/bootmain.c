/*************************************************************************
> File Name: bootmain.c
> Author: jiangnan
> Mail: kingo233@outlook.com
> Created Time: 2021年02月27日 星期六 15时16分45秒
************************************************************************/

#include <defs.h>
#include <x86.h>
#include <elf.h>

#define SECTSIZE        512
#define ELFHDR          ((struct elfhdr *)0x10000)      // scratch space

/* waitdisk - wait for disk ready */
static void
waitdisk(void) {
    while ((inb(0x1F7) & 0xC0) != 0x40)
    /* do nothing */;
}

/* readsect - read a single sector at @secno into @dst */
static void
readsect(void *dst, uint32_t secno) {
    // wait for disk to be ready
    waitdisk();

    outb(0x1F2, 1);                         // count = 1
    outb(0x1F3, secno & 0xFF);
    outb(0x1F4, (secno >> 8) & 0xFF);
    outb(0x1F5, (secno >> 16) & 0xFF);
    outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);
    outb(0x1F7, 0x20);                      // cmd 0x20 - read sectors

    // wait for disk to be ready
    waitdisk();

    // read a sector
    insl(0x1F0, dst, SECTSIZE / 4);
}

/* *
* readseg - read @count bytes at @offset from kernel into virtual address @va,
* might copy more than asked.
* */
static void
readseg(uintptr_t va, uint32_t count, uint32_t offset) {
    uintptr_t end_va = va + count;

    // round down to sector boundary
    va -= offset % SECTSIZE;

    // translate from bytes to sectors; kernel starts at sector 1
    uint32_t secno = (offset / SECTSIZE) + 1;

    // If this is too slow, we could read lots of sectors at a time.
    // We'd write more to memory than asked, but it doesn't matter --
    // we load in increasing order.
    for (; va < end_va; va += SECTSIZE, secno ++) {
        readsect((void *)va, secno);
    }
}
void 
bootmain(void){
    //把elf文件头读入进来
    readseg((uintptr_t) ELFHDR,8 * SECTSIZE,0);
    if(ELFHDR->e_magic != ELF_MAGIC){
        goto bad;
    }

    //program header
    struct proghdr* ph = (struct proghdr*) ((uintptr_t)ELFHDR + ELFHDR->e_phoff);
    struct proghdr* eph = (struct proghdr*) (ph + ELFHDR->e_phnum);
    for (; ph < eph; ph ++) {
        readseg(ph->p_va, ph->p_memsz, ph->p_offset);
    } 
    ( ( void (*)(void) )(ELFHDR->e_entry) ) ();
    bad:
    outw(0x8A00, 0x8A00);
    outw(0x8A00, 0x8E00);

    /* do nothing */
    while (1);
}
