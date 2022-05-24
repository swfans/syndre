#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "bflib_basics.h"
#include "bftext.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bfscrsurf.h"

#include "display.h"
#include "util.h"

extern char lbDrawAreaTitle[128];

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
        return "bfgame_icon.png";
    default:
        return NULL;
    }
}

#endif

static inline void
lock_screen (void)
{
    if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
        if (SDL_LockSurface (to_SDLSurf(lbScreenSurface)) != 0) {
            fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
            exit(1);
        }
    }
    // set vga buffer address
    lbDisplay.PhysicalScreen = to_SDLSurf(lbDrawSurface)->pixels;
}

static inline void
unlock_screen (void)
{
    if (SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface))) {
        SDL_UnlockSurface (to_SDLSurf(lbScreenSurface));
    }
    lbDisplay.PhysicalScreen = NULL;
}

void
display_lock (void)
{
  lock_screen ();
}

void swap_wscreen(void)
{
    TbBool has_wscreeen;
    has_wscreeen = (lbDisplay.WScreen != 0);
    if ( has_wscreeen )
        LbScreenUnlock();
    LbScreenSwap();
    if ( has_wscreeen )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}

void
display_unlock (void)
{
  unlock_screen ();
}
