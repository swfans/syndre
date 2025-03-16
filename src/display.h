/******************************************************************************/
// BullfrogGame Port, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file display.h
 *     Header file for display.c.
 * @par Purpose:
 *     Display ang graphics mode related function.
 * @par Comment:
 *     Extended wrappers for bflibrary functionalities.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stddef.h>

void display_lock (void);
void display_unlock (void);
const char * AppResourceMapping(short index);
void swap_wscreen(void);

/******************************************************************************/
#endif // DISPLAY_H
