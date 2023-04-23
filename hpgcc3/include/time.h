//& *** (c) 2006-2011 The HPGCC3 Team ***
//& Claudio Lapilli
//& Ingo Blank
//&
//& This file is licensed under the terms and conditions of the
//& HPGCC3 license that is included with the source distribution.
//& *** (c) 2006-2011 The HPGCC3 Team ***

// $Id: time.h 514 2007-11-18 03:11:26Z ingo $

/*!    \file    time.h
    \brief    The Time Library for HP49 gcc
*/

#ifndef _HPTIME_H
#define _HPTIME_H


#define CLOCKS_PER_SEC    60    

#ifdef _POSIX_SOURCE
#define CLK_TCK CLOCKS_PER_SEC    
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif


#ifndef _TIME_T
#define _TIME_T
typedef long time_t;        /* time in sec since 1 Jan 1970 0000 GMT */
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef long clock_t;        /* time in ticks since process started */
#endif

struct tm {
  int tm_sec;            /* seconds after the minute [0, 59] */
  int tm_min;            /* minutes after the hour [0, 59] */
  int tm_hour;            /* hours since midnight [0, 23] */
  int tm_mday;            /* day of the month [1, 31] */
  int tm_mon;            /* months since January [0, 11] */
  int tm_year;            /* years since 1900 */
  int tm_wday;            /* days since Sunday [0, 6] */
  int tm_yday;            /* days since January 1 [0, 365] */
  int tm_isdst;            /* Daylight Saving Time flag */
};


void sys_tm_RTC(struct tm *);

char *_isotime_r(const struct tm *, int strict_iso,char *buf);
#define isotime_r(tm,buf) _isotime_r((tm),0,(buf))
char *asctime_r(const struct tm *, char *buf); 


//extern char *tzname[];



#endif 
