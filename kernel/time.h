#ifndef __TIME_H__
#define __TIME_H__

#include <io.h>

#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

struct tm {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

inline __u8 cmos_read(__u8 addr){
    outb(0x70, 0x80|addr);
    return inb(0x71);
}

inline void get_time(struct tm *time)
{
	do {
		time->tm_sec = cmos_read(0);
		time->tm_min = cmos_read(2);
		time->tm_hour = cmos_read(4);
		time->tm_mday = cmos_read(7);
		time->tm_mon = cmos_read(8)-1;
		time->tm_year = cmos_read(9);
	} while (time->tm_sec != cmos_read(0));

	BCD_TO_BIN(time->tm_sec);
	BCD_TO_BIN(time->tm_min);
	BCD_TO_BIN(time->tm_hour);
	BCD_TO_BIN(time->tm_mday);
	BCD_TO_BIN(time->tm_mon);
	BCD_TO_BIN(time->tm_year);
}

#endif // __TIME_H__
