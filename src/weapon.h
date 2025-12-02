/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
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

enum WeaponType
{
  WEP_NULL = 0x0,
  WEP_PERSUADRTRN = 0x1,
  WEP_PISTOL = 0x2,
  WEP_GAUSSGUN = 0x3,
  WEP_SHOTGUN = 0x4,
  WEP_UZI = 0x5,
  WEP_MINIGUN = 0x6,
  WEP_LASER = 0x7,
  WEP_FLAMER = 0x8,
  WEP_LONGRANGE = 0x9,
  WEP_SCANNER = 0xA,
  WEP_MEDIKIT = 0xB,
  WEP_TIMEBOMB = 0xC,
  WEP_ACCESSCARD = 0xD,
  WEP_ACCESSCRD1 = 0xE,
  WEP_ACCESSCRD2 = 0xF,
  WEP_AUTOMAPPER = 0x10,
  WEP_ENERGYSHLD = 0x11,
  WEP_SHIELD1 = 0x12,
  WEP_SHIELD2 = 0x13,
  WEP_TYPES_COUNT,
};

enum CyModType
{
  MOD_NULL = 0x0,
  MOD_LEGS1 = 0x1,
  MOD_LEGS2 = 0x2,
  MOD_LEGS3 = 0x3,
  MOD_ARMS1 = 0x4,
  MOD_ARMS2 = 0x5,
  MOD_ARMS3 = 0x6,
  MOD_CHEST1 = 0x7,
  MOD_CHEST2 = 0x8,
  MOD_CHEST3 = 0x9,
  MOD_UNKN17 = 0x11,
  MOD_TYPES_COUNT,
};

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

extern struct EquipmentInfo weapons[WEP_TYPES_COUNT+1];

extern struct PartsInfo cybmods[MOD_TYPES_COUNT+1];

#pragma pack()
/******************************************************************************/


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
