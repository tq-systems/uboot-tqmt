/*
 * (C) Copyright 2015, TQ Systems GmbH
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Date & Time support for NXP PCF85063 RTC
 */

/* #define	DEBUG	*/

#include <common.h>
#include <command.h>
#include <rtc.h>
#include <i2c.h>

#if defined(CONFIG_CMD_DATE)

static uchar rtc_read  (uchar reg);
static void  rtc_write (uchar reg, uchar val);

/* ------------------------------------------------------------------------- */

int rtc_get (struct rtc_time *tmp)
{
	int rel = 0;
	uchar ctrl1, sec, min, hour, mday, wday, mon, year;

	ctrl1 = rtc_read (0x00);
	sec   = rtc_read (0x04);
	min   = rtc_read (0x05);
	hour  = rtc_read (0x06);
	mday  = rtc_read (0x07);
	wday  = rtc_read (0x08);
	mon   = rtc_read (0x09);
	year  = rtc_read (0x0a);

	debug ( "Get RTC year: %02x mon: %02x mday: %02x wday: %02x "
		"hr: %02x min: %02x sec: %02x, ctrl: %02x\n",
		year, mon, mday, wday,
		hour, min, sec, ctrl1 );
	
	if (sec & 0x80) {
		puts ("### Warning: RTC Low Voltage - date/time not reliable\n");
		rel = -1;
	}

	tmp->tm_sec  = bcd2bin (sec  & 0x7F);
	tmp->tm_min  = bcd2bin (min  & 0x7F);
	/* 12/24 hour format handling */
	if(ctrl1 & 0x02) {
		tmp->tm_hour = bcd2bin (hour & 0x3F);
	 } else {
		tmp->tm_hour = bcd2bin (hour & 0x1F);
		if(hour & 0x20) {
			/* handle AM/PM */
			tmp->tm_hour += 12;
		}
	}
	tmp->tm_mday = bcd2bin (mday & 0x3F);
	tmp->tm_mon  = bcd2bin (mon & 0x1F);
	tmp->tm_year = bcd2bin (year);
	/* handle century (expect date within 1970 - 2069) */
	if(tmp->tm_year < 70)
		tmp->tm_year += 2000;
	else
		tmp->tm_year += 1900;
	tmp->tm_wday = bcd2bin (wday & 0x07);
	tmp->tm_yday = 0;
	tmp->tm_isdst= 0;

	debug ( "Get DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	return rel;
}

int rtc_set (struct rtc_time *tmp)
{
	uchar ctrl1;

	ctrl1 = rtc_read (0x00);

	debug ( "Set DATE: %4d-%02d-%02d (wday=%d)  TIME: %2d:%02d:%02d\n",
		tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_wday,
		tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

	rtc_write (0x0a, bin2bcd(tmp->tm_year % 100));
	rtc_write (0x09, bin2bcd(tmp->tm_mon));
	rtc_write (0x08, bin2bcd(tmp->tm_wday));
	rtc_write (0x07, bin2bcd(tmp->tm_mday));
        /* 12/24 hour format handling */
        if(ctrl1 & 0x02) {
		rtc_write (0x06, bin2bcd(tmp->tm_hour));
        } else {
		if(tmp->tm_hour >= 12)
	               rtc_write (0x06, bin2bcd(tmp->tm_hour % 12) | 0x20);
		else
                       rtc_write (0x06, bin2bcd(tmp->tm_hour % 12));
        }
	rtc_write (0x05, bin2bcd(tmp->tm_min));
	rtc_write (0x04, bin2bcd(tmp->tm_sec));

	return 0;
}

void rtc_reset (void)
{
	/* issue software reset commend*/
	rtc_write(0x00, 0x58);
}

/* ------------------------------------------------------------------------- */

static uchar rtc_read (uchar reg)
{
	return (i2c_reg_read (CONFIG_SYS_I2C_RTC_ADDR, reg));
}

static void rtc_write (uchar reg, uchar val)
{
	i2c_reg_write (CONFIG_SYS_I2C_RTC_ADDR, reg, val);
}

#endif
