#include <vga.h>

static __u8 m_color = 0x0F;
static __u8 m_row = 0;
static __u8 m_col = 0;

void vga_color(__u8 color)
{
    m_color = color;
}

// FIXME: 使用汇编替换，保证高效处理
void vga_clear()
{
    __u8 chr = ' ';
    int row = 0;
    int col = 0;

    for (row=0; row<25; row++) {
        for (col=0; col<80; col++) {
            __u32 edi = (row*80+col)*2;
            __asm__ ("movb %0,    %%al;"
                     "movb %1,    %%ah;"
                     "movl %2,    %%edi;"
                     "mov  %%ax,  %%gs:(%%edi);"
                     ::"m"(chr), "m"(m_color), "m"(edi));
        }
    }

    m_row = 0;
    m_col = 0;
}

void vga_put_xy(__u8 chr, __u8 row, __u8 col)
{
    __u32 edi = (row*80+col)*2;

    __asm__ ("movb %0,    %%al;"
             "movb %1,    %%ah;"
             "movl %2,    %%edi;"
             "mov  %%ax,  %%gs:(%%edi);"
             ::"m"(chr), "m"(m_color), "m"(edi));
}

void vga_put(__u8 chr)
{
    if (m_col >= 80) {
        m_col = 0;
        m_row++;
    }
    if (m_row >= 25) {
        vga_clear();
        m_row = 0;
        m_col = 0;
    }

    __u32 edi = (m_row*80+m_col)*2;

    __asm__ ("movb %0,    %%al;"
             "movb %1,    %%ah;"
             "movl %2,    %%edi;"
             "mov  %%ax,  %%gs:(%%edi);"
             ::"m"(chr), "m"(m_color), "m"(edi));

    m_col++;
}

void vga_goto(__u8 row, __u8 col)
{
    m_row = row;
    m_col = col;
}

void vga_crlf()
{
    m_col = 0;
    m_row++;
    if (m_row >= 25) {
        vga_clear();
        m_row = 0;
    }
}

void vga_string(char *str)
{
    static int i = 0;
    for (i=0; str[i] != '\0'; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
            vga_crlf();
        }
        else {
            vga_put(str[i]);
        }
    }
}
