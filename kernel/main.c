#include <types.h>
#include <vga.h>
#include <time.h>

void print_string(char *str)
{
    int i = 0;
    for (i=0; str[i] != '\0'; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
        }
        else {
            vga_put(str[i]);
        }
    }
}

__s32 main(__s32 argc, __s8 ** argv)
{
    vga_color(COLOR_BG_GREEN|COLOR_FG_RED);
    vga_clear();

    vga_goto(0, 0);
    print_string("Now I'm In Main!\0");

    vga_goto(1, 0);
    print_string("I'm In Main Too!\0");

LOOP:
    goto LOOP;

    return 0;
}

