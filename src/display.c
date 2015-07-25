#include <stdint.h>
#include <stdio.h>

#include "display.h"
#include "util.h"

static SDL_Surface *display_screen;

static inline void
lock_screen (void)
{
  if (!SDL_MUSTLOCK (display_screen))
    return;

  if (SDL_LockSurface (display_screen) != 0)
    fprintf (stderr, "SDL_LockSurface: %s\n", SDL_GetError ());
}

static inline void
unlock_screen (void)
{
  if (!SDL_MUSTLOCK (display_screen))
    return;

  SDL_UnlockSurface (display_screen);
}

int
display_set_palette (const uint8_t *palette)
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

  if (SDL_SetPalette (display_screen,
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
  SDL_FreeSurface (display_screen);
  display_screen = NULL;
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
