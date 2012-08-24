#ifndef _VGA_H_
#define _VGA_H_

#include <types.h>

#define COLOR_FG_BLACK  0x00
#define COLOR_FG_BLUE   0x01
#define COLOR_FG_GREEN  0x02
#define COLOR_FG_3      0x03
#define COLOR_FG_RED    0x04
#define COLOR_FG_5      0x05
#define COLOR_FG_6      0x06
#define COLOR_FG_7      0x07

#define COLOR_FG_HL     0x08

#define COLOR_BG_BLACK  0x00
#define COLOR_BG_BLUE   0x10
#define COLOR_BG_GREEN  0x20
#define COLOR_BG_3      0x30
#define COLOR_BG_RED    0x40
#define COLOR_BG_5      0x50
#define COLOR_BG_6      0x60
#define COLOR_BG_7      0x70

#define COLOR_BG_BLK    0x80

void vga_color(__u8 color);

void vga_clear();

void vga_put_xy(__u8 chr, __u8 row, __u8 col);

void vga_goto(__u8 row, __u8 col);
void vga_crlf();

void vga_put(__u8 chr);

void vga_string(char *str);

#endif
