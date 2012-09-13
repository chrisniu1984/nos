#include <types.h>
#include <vga.h>
#include <time.h>
#include <idt.h>

int main(void)
{
    vga_color(COLOR_FG_RED|COLOR_BG_BLACK);
    vga_clear();
    vga_string("Now I am in main()\n\0");

    idt_init();

    //__u32 a = 10/0; // 测试div_zero

LOOP:
    goto LOOP;
}
