#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bftext.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bfscrsurf.h"

#include "applog.h"
#include "display.h"
#include "util.h"


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

    // Mapping of video modes
    if (mode == 19)
        mode = Lb_SCREEN_MODE_320_200_8;

    mdinfo = LbScreenGetModeInfo(mode);
    LOGSYNC("Entering mode %d, %dx%d", (int)mode, (int)mdinfo->Width, (int)mdinfo->Height);
    return LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, GraphicsPalette);
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

void
display_lock (void)
{
  LbScreenLock();
}

void
display_unlock (void)
{
  LbScreenUnlock();
}

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    if ( was_locked )
        LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}
