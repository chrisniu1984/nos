#ifndef __IO_H__
#define __IO_H__

static inline void outb(__u8 port, __u8 val)
{
    __asm__ ("outb %%al,%%dx"
             :
             :"a" (val), "d" (port)
             :);
}

static inline __u8 inb(__u8 port) 
{
    __u8 val;
    __asm__ volatile ("inb %%dx,%%al"
                      :"=a" (val)
                      :"d" (port)
                      :);

    return val;
}


#define cli()   __asm__("cli;":::)
#define sti()   __asm__("sti;":::)

#define eoi()   outb(0x20, 0x20)

#endif // __IO_H__
