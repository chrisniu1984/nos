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
#define COLOR_FG_NO_HL  0x00

#define COLOR_BG_BLACK  0x00
#define COLOR_BG_BLUE   0x01
#define COLOR_BG_GREEN  0x02
#define COLOR_BG_3      0x03
#define COLOR_BG_RED    0x04
#define COLOR_BG_5      0x05
#define COLOR_BG_6      0x06
#define COLOR_BG_7      0x07

#define COLOR_BG_BLK    0x80
#define COLOR_BG_NO_BLK 0x00

void vga_set(__u32 c, __u8 row, __u8 col);

void clear(void *s, __s8 c, __u32 count);

#endif
