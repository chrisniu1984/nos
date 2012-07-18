#ifndef __IO_H__
#define __IO_H__

inline void outb(port, val)
{
    __asm__ ("outb %%al,%%dx"
             :
             :"a" (val),"d" (port));
}

inline __u8 inb(port) 
{
    unsigned char val;
    __asm__ volatile ("inb %%dx,%%al"
                      :"=a" (val)
                      :"d" (port));

    return val;
}

#endif // __IO_H__
