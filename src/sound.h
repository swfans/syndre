/******************************************************************************/
// BullfrogGame Port, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file sound.h
 *     Header file for sound.c.
 * @par Purpose:
 *     Sound samples playback wrappers.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 13 Oct 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "bftypes.h"
#include "timer.h"
#include "mssal.h"

/******************************************************************************/
#pragma pack(1)

#pragma pack()
/******************************************************************************/

extern short startscr_samplevol;
extern short startscr_midivol;
extern short startscr_cdvolume;

int LoadMusic(ushort bankNo);

void fill_ail_sample_ids(void);

/******************************************************************************/
#endif // SOUND_H
