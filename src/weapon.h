/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file weapon.h
 *     Header file for weapon.c.
 * @par Purpose:
 *     Weapons related functions.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef WEAPON_H
#define WEAPON_H

#include "bftypes.h"

#include "game_bstype.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct EquipmentInfo {
    ubyte text_id;
    int field_1;
    ushort field_5;
    int field_7;
    ushort field_B;
    int field_D;
    ushort field_11;
    ushort field_13[10][24];
    ubyte field_1F3;
    ubyte Category;
};

struct PartsInfo {
    ubyte text_id;
    int Cost;
    ubyte field_5;
    ushort field_6;
    ushort field_8;
    ushort field_A[240];
    ubyte field_1EA;
};

extern struct EquipmentInfo weapons[21];

extern struct PartsInfo cybmods[19];

#pragma pack()
/******************************************************************************/


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
