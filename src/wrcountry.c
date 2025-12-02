/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file wrcountry.c
 *     Routines for handling a list of countries for world map.
 * @par Purpose:
 *     Implement functions for parsing country file and handling countries array.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 11 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "wrcountry.h"

#include <string.h>
#include <stdlib.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfstrut.h"
#include "bfutility.h"

#include "game.h"
#include "game_data.h"
#include "guitext.h"
#include "applog.h"
/******************************************************************************/


/******************************************************************************/
