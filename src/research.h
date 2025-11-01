/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file research.h
 *     Header file for research.c.
 * @par Purpose:
 *     Research state and progress support.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     19 Apr 2022 - 27 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef RESEARCH_H
#define RESEARCH_H

#include "bftypes.h"

#include "game_bstype.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct struc_1E9_sub1 {
  ushort field_0;
  ushort field_2[22];
  ushort field_2E;
};

struct Research {
    ubyte field_0;
    ubyte CurrentKind;
    ubyte WMType;
    int field_3;
    struct struc_1E9_sub1 field_7[10];
    ushort field_1E7;
};

#pragma pack()
/******************************************************************************/
extern struct Research research;

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
