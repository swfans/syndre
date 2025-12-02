/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file mouse.c
 *     Handling mouse position and mouse pointer sprites.
 * @par Purpose:
 *     Functions which wrap bflibrary routines for higher level mouse functions.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 11 Oct 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

#include "mouse.h"

#include "bfscreen.h"
#include "bfplanar.h"
#include "bfmouse.h"
#include "bfutility.h"

#include "display.h"

void mouse_update_scaled_coords(void)
{
    if (DrawFlags & DrwF_ScreenVres16) {
        lbDisplay__MouseX_640 = lbDisplay.MouseX;
        lbDisplay__MouseY_400 = max(lbDisplay.MouseY - 40, 0);
        lbDisplay__MMouseX_640 = lbDisplay.MMouseX;
        lbDisplay__MMouseY_400 = max(lbDisplay.MMouseY - 40, 0);
    } else {
        lbDisplay__MouseX_640 = lbDisplay.MouseX * 2;
        lbDisplay__MouseY_400 = lbDisplay.MouseY * 2;
        lbDisplay__MMouseX_640 = lbDisplay.MMouseX * 2;
        lbDisplay__MMouseY_400 = lbDisplay.MMouseY * 2;
    }
    if (lbDisplay__MMouseY_400 > 399)
        lbDisplay__MMouseY_400 = 399;
}

/******************************************************************************/
