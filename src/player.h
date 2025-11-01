/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file player.h
 *     Header file for player.c.
 * @par Purpose:
 *     Player state and information handling.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "bftypes.h"
#include "game_bstype.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

#define PLAYERS_LIMIT 8

struct Thing;

struct EquipWeapon {
    ushort ammo;
    ushort wtype;
};

struct PlayerCyborg {
    ubyte NameId;
    ushort State;
    ushort Equipment;
    ushort field_5;
    ubyte SelctTeamSlot;
    struct EquipWeapon Weapons[7];
    ushort field_24;
    ushort field_26;
};

struct PlayerInfo {
    uint Credits;
    uint field_4;
    ushort field_8;
    ushort field_A;
    ushort field_C;
    ubyte FlagsE;
    ubyte OwnColourScheme;
    ubyte OwnLogoShape;
    ubyte LeaderName[18];
    ubyte CompanyName[18];
    ubyte field_35[128];
    ubyte field_B5;
    ubyte field_B6;
    ushort field_B7;
    ubyte field_B9[50];
    ubyte field_EB[50];
    struct PlayerCyborg Cyborgs[18];
    ubyte field_3ED[30];
    ushort field_40B;
    ubyte field_40D[8];
    ushort field_415;
};


extern struct PlayerInfo players[PLAYERS_LIMIT];
extern ubyte selected_team[4];
extern ushort Network__Slot;

#pragma pack()
/******************************************************************************/

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
