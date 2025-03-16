/******************************************************************************/
// BullfrogGame Port, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file oswindows.h
 *     Header file for windows.c.
 * @par Purpose:
 *     Implementation of few functions which is specific to Windows OS.
 * @par Comment:
 *     None.
 * @author   Gynvael Coldwind
 * @author   Unavowed
 * @date     10 Oct 2012 - 13 Oct 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef OSWINDWS_H
#define OSWINDWS_H

#ifdef WIN32
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

/******************************************************************************/

#ifndef HAVE_GETTIMEOFDAY
struct timezone
{
  int  tz_minuteswest;
  int  tz_dsttime;
};

int gettimeofday (struct timeval *tp, struct timezone *tzp);
#endif

bool sys_get_user_path (char *buffer, size_t size);
bool sys_get_data_path (char *buffer, size_t size);

/******************************************************************************/
#endif // WIN32

#endif // OSWINDWS_H
