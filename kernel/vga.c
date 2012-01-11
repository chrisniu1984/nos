#include <drv/vga.h>

static __u8 row = 0;
static __u8 col = 0;

void vga_set(__u32 c, __u8 row, __u8 col)
{
    __u32 edi = (row*80+col)*2;

    __asm__("movl %1, %%edi;"
            "mov %0, %%eax;"
            "movb $0x0C, %%ah;"
            "mov %%ax, %%gs:(%%edi);"
            :
            :"m"(c), "m"(edi));
}

void clear(void *s, __s8 c, __u32 count)
{
    __asm__("cld\n\t"
            "rep\n\t"
            "stosb"
            ::"a" (c),"D" (s),"c" (count)
            :"cx","di");
    return s;
}
