#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bftext.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfmouse.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bfscrsurf.h"

#include "applog.h"
#include "display.h"
#include "util.h"

ubyte *VGABuffer = NULL;

#if defined(WIN32)

const char * AppResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "A";
        //return MAKEINTRESOURCE(110); -- may work for other resource compilers
    default:
        return NULL;
    }
}

#else

const char * AppResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "syndre_icon64.png";
    default:
        return NULL;
    }
}

#endif

TbResult AppScreenSetup(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    TbResult ret;
    ushort ratio_mul;

    ratio_mul = 2;
    // Mapping of video modes
    if (mode == 19)
        mode = Lb_SCREEN_MODE_320_200_8;
    else if (mode == 18)
        mode = Lb_SCREEN_MODE_640_480_8;
    lbDisplay.WScreen = VGABuffer + 640*480;

    mdinfo = LbScreenGetModeInfo(mode);
    LOGSYNC("Entering mode %d, %dx%d", (int)mode, (int)mdinfo->Width, (int)mdinfo->Height);
    ret = LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, GraphicsPalette);
    // This app requires mouse window larger than screen resolution
    // For the ration change to have any effect, the project nees to be
    // built with `--enable-lb-mouse-move-ratio`
    LbMouseChangeMoveRatio(ratio_mul << 8, ratio_mul << 8);
    LbMouseSetWindow(0, 0, lbDisplay.GraphicsScreenWidth * ratio_mul,
      lbDisplay.GraphicsScreenHeight * ratio_mul);
    return ret;
}

void display_set_full_screen(bool full_screen)
{
    int i;

    if (lbScreenSurface != NULL)
        return;

    for (i = 1; i < LB_MAX_SCREEN_MODES_COUNT; ++i)
    {
        TbScreenModeInfo *mdinfo;

        mdinfo = LbScreenGetModeInfo(i);
        if (mdinfo->Width == 0) break;
        if (full_screen) {
            mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
        } else {
            mdinfo->VideoMode |= Lb_VF_WINDOWED;
        }
    }
}

void display_set_lowres_stretch(bool stretch)
{
    if (stretch)
        LbScreenSetMinScreenSurfaceDimension(400);
    else
        LbScreenSetMinScreenSurfaceDimension(1);
}

void display_create_vga_buffer(void)
{
    // First for 4bpp buffer, second for 8BPP WScreen buffer
    VGABuffer = LbMemoryAlloc(640*480 * 2 + 16);
}

void display_free_vga_buffer(void)
{
    LbMemoryFree(VGABuffer);
    VGABuffer = NULL;
}

void display_lock(void)
{
    LbScreenLock();
}

void display_unlock(void)
{
    LbScreenUnlock();
}

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    LbScreenLock();
    if (DrawFlags & DrwF_ScreenVres16)
        LbMemoryCopy(lbDisplay.WScreen, VGABuffer, 640*480);
    else
        LbMemoryCopy(lbDisplay.WScreen, WScreen, 320*200);
    LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}
