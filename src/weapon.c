/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file weapon.c
 *     Weapons related functions.
 * @par Purpose:
 *     Implement functions for handling weapons.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "weapon.h"

#include "bfmemory.h"
#include "bffile.h"
#include "bfutility.h"

#include "player.h"
#include "game.h"
#include "game_data.h"
#include "sound.h"
#include "applog.h"
/******************************************************************************/


/******************************************************************************/
