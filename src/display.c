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

static inline void
lock_screen (void)
{
  if (!SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface)))
    return;

  if (SDL_LockSurface (to_SDLSurf(lbDrawSurface)) != 0)
    fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
}

static inline void
unlock_screen (void)
{
  if (!SDL_MUSTLOCK (to_SDLSurf(lbDrawSurface)))
    return;

  SDL_UnlockSurface (to_SDLSurf(lbDrawSurface));
}

void
display_initialise (void)
{
  SDL_WM_SetCaption ("BullfrogGame", NULL);
}

void
display_finalise (void)
{
  //unlock_screen (); -- handled in LbScreenReset()
  //SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
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
