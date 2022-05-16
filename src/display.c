#include <stdint.h>
#include <stdio.h>

#include "bflib_basics.h"
#include "bftext.h"
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

TbResult
LbPaletteSet(const unsigned char *palette)
{
  SDL_Color colours[256];
  int n;

  for (n = 0; n < 256; n++)
    {
      colours[n].r = palette[3 * n + 0] * 4;
      colours[n].g = palette[3 * n + 1] * 4;
      colours[n].b = palette[3 * n + 2] * 4;
      colours[n].unused = 0;
    }

  if (SDL_SetPalette (to_SDLSurf(lbDrawSurface),
		      SDL_LOGPAL | SDL_PHYSPAL, colours, 0, 256) != 1)
    {
      fprintf (stderr, "SDL_SetPalette: %s\n", SDL_GetError ());
      return -1;
    }

  memcpy (display_palette, colours, sizeof (display_palette));

  return 1;
}

void
display_initialise (void)
{
  SDL_WM_SetCaption ("BullfrogGame", NULL);
}

void
display_finalise (void)
{
  unlock_screen ();
  SDL_FreeSurface (to_SDLSurf(lbDrawSurface));
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
