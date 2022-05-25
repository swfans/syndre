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
#if 0
    if (SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface))) {
        if (SDL_LockSurface (to_SDLSurf(lbDrawSurface)) != 0) {
            fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
            exit(1);
        }
    }
#endif
}

static inline void
unlock_screen (void)
{
#if 0
    if (SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface))) {
        SDL_UnlockSurface (to_SDLSurf(lbDrawSurface));
    }
#endif
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
