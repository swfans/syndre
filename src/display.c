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

void update_vscreen_whole_vres16(void)
{
    const ushort *i;
    ushort *o;
    int n, k;

    LbMemorySet(VScreen, 0, 320*400);

    i = (const ushort *)WScreen;
    o = (ushort *)VScreen;
    for (k = 50; k > 0; k--)
    {
        for (n = 320; n > 0; n--)
        {
            *o |= (*i) & 0x1111;
            o++;
            i++;
        }

        i += 320 * 49;
        o -= 320;
        for (n = 320; n > 0; n--)
        {
            *o |= (*i) & 0x2222;
            o++;
            i++;
        }

        i += 320 * 49;
        o -= 320;
        for (n = 320; n > 0; n--)
        {
            *o |= (*i) & 0x4444;
            o++;
            i++;
        }

        i += 320 * 49;
        o -= 320;
        for (n = 320; n > 0; n--)
        {
            *o |= (*i) & 0x8888;
            o++;
            i++;
        }
        i -= 320 * 150;
    }
}

void blit_wscreen_vres16(const ubyte *buf)
{
    const ubyte *i;
    ubyte *o;
    int n, k;

    LbMemorySet(lbDisplay.WScreen, 0, 640*480);

    i = buf;
    o = lbDisplay.WScreen;
    for (k = 480; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) >> 0) & 0x01;
            o[6] |= ((*i) >> 1) & 0x01;
            o[5] |= ((*i) >> 2) & 0x01;
            o[4] |= ((*i) >> 3) & 0x01;
            o[3] |= ((*i) >> 4) & 0x01;
            o[2] |= ((*i) >> 5) & 0x01;
            o[1] |= ((*i) >> 6) & 0x01;
            o[0] |= ((*i) >> 7) & 0x01;
            o += 8;
            i++;
        }
    }

    o = lbDisplay.WScreen;
    for (k = 480; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 1) & 0x02;
            o[6] |= ((*i) >> 0) & 0x02;
            o[5] |= ((*i) >> 1) & 0x02;
            o[4] |= ((*i) >> 2) & 0x02;
            o[3] |= ((*i) >> 3) & 0x02;
            o[2] |= ((*i) >> 4) & 0x02;
            o[1] |= ((*i) >> 5) & 0x02;
            o[0] |= ((*i) >> 6) & 0x02;
            o += 8;
            i++;
        }
    }

    o = lbDisplay.WScreen;
    for (k = 480; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 2) & 0x03;
            o[6] |= ((*i) << 1) & 0x03;
            o[5] |= ((*i) >> 0) & 0x03;
            o[4] |= ((*i) >> 1) & 0x03;
            o[3] |= ((*i) >> 2) & 0x03;
            o[2] |= ((*i) >> 3) & 0x03;
            o[1] |= ((*i) >> 4) & 0x03;
            o[0] |= ((*i) >> 5) & 0x03;
            o += 8;
            i++;
        }
    }

    o = lbDisplay.WScreen;
    for (k = 480; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 3) & 0x04;
            o[6] |= ((*i) << 2) & 0x04;
            o[5] |= ((*i) << 1) & 0x04;
            o[4] |= ((*i) >> 0) & 0x04;
            o[3] |= ((*i) >> 1) & 0x04;
            o[2] |= ((*i) >> 2) & 0x04;
            o[1] |= ((*i) >> 3) & 0x04;
            o[0] |= ((*i) >> 4) & 0x04;
            o += 8;
            i++;
        }
    }
}

void blit_wscreen_mcga(const ubyte *buf)
{
    LbMemoryCopy(lbDisplay.WScreen, buf, 320*200);
}

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    LbScreenLock();
    if (DrawFlags & DrwF_ScreenVres16)
        blit_wscreen_vres16(VGABuffer);
    else
        blit_wscreen_mcga(WScreen);
    LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}
