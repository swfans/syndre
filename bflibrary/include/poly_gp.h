/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file poly_gp.h
 *     Header file for poly_gp.c.
 * @par Purpose:
 *     Textured 2D triangle rendring, clipping and preparation.
 * @par Comment:
 *     To be used internally; to draw a triangle, use API from `poly.h`.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 05 Nov 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef POLY_GP_H_
#define POLY_GP_H_

#include "poly.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)

struct gpoly_point {
    int Y;
    int X;
    int X_prc;
    int S;
    int U;
    int V;
};

struct gpoly_factors {
    int S[4];
};

struct gpoly_blends {
    int B[3];
};

struct gpoly_state {
    int ratioCA_X2Y;
    int ratioBA_X2Y;
    int ratioCB_X2Y;
    int var_134;
    struct gpoly_point ptA;
    struct gpoly_point ptB;
    struct gpoly_point ptC;
    struct gpoly_factors incC;
    struct gpoly_factors incD;
    struct gpoly_factors incA;
    struct gpoly_factors incB;
    struct gpoly_blends bldA;
    struct gpoly_blends bldB;
};

#pragma pack()

ubyte gpoly_reorder_input_points(struct PolyPoint **opt_a,
  struct PolyPoint **opt_b, struct PolyPoint **opt_c);
void gpoly_init_state(struct gpoly_state *st, struct PolyPoint *point_a,
  struct PolyPoint *point_b, struct PolyPoint *point_c);

void gpoly_sta_md03(struct gpoly_state *st);
void gpoly_sta_md04(struct gpoly_state *st);
void gpoly_sta_md05(struct gpoly_state *st);
void gpoly_sta_md27(struct gpoly_state *st);
void gpoly_sta_md28(struct gpoly_state *st);

/**
 * Renders shaded gpoly triangle using given state, checking window bounds.
 */
void gpoly_rasterize_shaded_bound(struct gpoly_state *st);

/**
 * Renders shaded gpoly triangle using given state, without bounds check.
 */
void gpoly_rasterize_shaded_nobound(struct gpoly_state *st);

/**
 * Renders const brightness gpoly triangle using given state, checking window bounds.
 */
void gpoly_rasterize_noshade_bound(struct gpoly_state *st);

/**
 * Renders const brightness gpoly triangle using given state, without bounds check.
 */
void gpoly_rasterize_noshade_nobound(struct gpoly_state *st);


#ifdef __cplusplus
};
#endif

#endif // POLY_GP_H_
/******************************************************************************/
