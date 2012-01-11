#include <types.h>

const __u8 DIGIT[16] = "0123456789ABCDEF";
extern __u32 _GDT;
extern __u16 _GDT_LIMIT;
extern __u32 _IDT;
extern __u16 _IDT_LIMIT;

__s32 main(__s32 argc, __s8 ** argv)
{
LOOP:
    goto LOOP;

    return 0;
}

