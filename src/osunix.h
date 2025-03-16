/******************************************************************************/
// BullfrogGame Port, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file osunix.h
 *     Header file for unix.c.
 * @par Purpose:
 *     Implementation of few functions which is specific to UNIX systems.
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
#ifndef OSUNIX_H
#define OSUNIX_H

#ifdef __unix__

#include <stdbool.h>

/******************************************************************************/

void unix_restore_signal_handlers (void);
bool sys_get_user_path (char *buffer, size_t size);
bool sys_get_data_path (char *buffer, size_t size);

/******************************************************************************/

#endif

#endif // OSUNIX_H
