/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game_data.h
 *     Header file for game_data.c.
 * @par Purpose:
 *     Structures and functions defining data used by the game.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     10 Feb 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "bftypes.h"

#pragma pack(1)

struct Thing;

struct CPObjective
{
    short Child;
    ushort Parent;
    short field_4;
    ubyte field_6;
    ubyte ActionType;
    ubyte Action;
    ushort TargetOffs;
    short field_B;
    short field_D;
};

#pragma pack()

/******************************************************************************/
extern ushort level__Seed;
extern struct CPObjective level__CPObjectives[128];


const char *GetDirectoryUser(void);
const char *GetDirectoryHdd(void);

void setup_file_names(void);

/******************************************************************************/
#endif // GAME_DATA_H
