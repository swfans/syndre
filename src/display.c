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
  if (!SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface)))
    return;

  if (SDL_LockSurface (to_SDLSurf(lbScreenSurface)) != 0)
    fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
}

static inline void
unlock_screen (void)
{
  if (!SDL_MUSTLOCK (to_SDLSurf(lbScreenSurface)))
    return;

  SDL_UnlockSurface (to_SDLSurf(lbScreenSurface));
}

void
display_lock (void)
{
  lock_screen ();
}

void
display_unlock (void)
{
  unlock_screen ();
}
