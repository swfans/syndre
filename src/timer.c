/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file timer.h
 *     Time counting and time related events implementation.
 * @par Purpose:
 *     Additional app-specific time related routines, expanding what is
 *     available in bflibrary.
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
#include <SDL.h>
#include <assert.h>
#include <stdint.h>

#include "timer.h"

#include "sound.h"

/******************************************************************************/

uint32_t
timer_get_18_2_hz_ticks (void)
{
  return (((uint64_t) SDL_GetTicks () * 1193182 / 1000) >> 15);
}

uint32_t
MSSSMACKTIMERREAD (void)
{
  return SDL_GetTicks ();
}

/******************************************************************************/
