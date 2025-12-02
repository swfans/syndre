/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file wrcountry.h
 *     Header file for wrcountry.c.
 * @par Purpose:
 *     Routines for handling a list of countries for world map.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 11 Sep 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef WRCOUNTRY_H
#define WRCOUNTRY_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct CountryState {
  short Happiness;
  ubyte OwningPlayer;
  char Revenue;
  char TaxPercent;
  ubyte field_5;
  uint Population;
};

extern struct CountryState country_states[50];

#pragma pack()
/******************************************************************************/


/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
