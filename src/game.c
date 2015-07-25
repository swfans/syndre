#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <png.h>

#include "display.h"
#include "dos.h"
#include "game.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "unix.h"
#include "util.h"
#include "windows.h"

#include "timer.h"

#define SAVEGAME_PATH "savegame/"

static char *game_user_path;
static char *game_data_path;

static bool
create_user_directory (void)
{
  char buffer[FILENAME_MAX];

  if (!sys_get_user_path (buffer, sizeof (buffer)))
    {
      game_user_path = xstrdup ("savegame");
      return true;
    }

  game_user_path = xstrdup (buffer);

  return mkdir_recursive (game_user_path, 0700);
}

bool
game_initialise (void)
{
  char buffer[FILENAME_MAX];


  if (SDL_Init (SDL_INIT_JOYSTICK | SDL_INIT_VIDEO
		| SDL_INIT_NOPARACHUTE) != 0)
    {
      fprintf (stderr, "SDL_Init(): %s\n", SDL_GetError ());
      return false;
    }

#ifdef __unix__
  /* clean up after SDL messing around where it shouldn't */
  unix_restore_signal_handlers ();
#endif

  keyboard_initialise ();
  mouse_initialise ();
  display_initialise ();
  sound_initialise ();

  if (sys_get_data_path (buffer, sizeof (buffer)))
    game_data_path = xstrdup (buffer);
  else
    game_data_path = xstrdup (".");

  create_user_directory ();

  return true;
}

void
game_handle_sdl_events (void)
{
  const size_t max_events = 256;
  size_t event;
  SDL_Event ev;

  for (event = 0; event < max_events; event++)
    {
      if (SDL_PollEvent (&ev) == 0)
	break;

      switch (ev.type)
	{
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
	  mouse_handle_event (&ev);
	  break;

	case SDL_KEYUP:
	case SDL_KEYDOWN:
	  keyboard_handle_event (&ev);
	  break;

	case SDL_QUIT:
	  game_quit ();
	}
    }
}

void
game_quit (void)
{
  xfree (game_user_path);
  xfree (game_data_path);
  sound_finalise ();
  display_finalise ();
  SDL_Quit ();
  exit (0);
}

void
game_transform_path_full (const char *file_name, char *buffer, size_t size)
{
  if (strncasecmp (file_name, SAVEGAME_PATH,
		   sizeof (SAVEGAME_PATH) - 1) == 0)
    {
      snprintf (buffer, size, "%s" FS_SEP_STR "%s", game_user_path,
		file_name + sizeof (SAVEGAME_PATH) - 1);
      return;
    }

  /* abort on absolute paths */
  if (file_name[0] == '\\' || file_name[0] == '/'
      || (strlen (file_name) >= 2 && file_name[1] == ':'))
    {
      snprintf (buffer, size, "%s", file_name);
      return;
    }

  snprintf (buffer, size, "%s" FS_SEP_STR "%s", game_data_path, file_name);
}

void
game_transform_path (const char *file_name, char *result)
{
  game_transform_path_full (file_name, result, FILENAME_MAX);
}

const char *
game_get_data_path (void)
{
  return game_data_path;
}

const char *
game_get_user_path (void)
{
  return game_user_path;
}

ssize_t
game_file_length (const char *filename)
{
  struct stat st;
  char transformed[FILENAME_MAX];
  char path[FILENAME_MAX];

  game_transform_path_full (filename, transformed, sizeof (transformed));
  dos_path_to_native (transformed, path, sizeof (path));

  if (stat (path, &st) != 0)
    {
      perror (path);
      return -1;
    }

  return st.st_size;
}

int
game_wait_for_vsync (void)
{
  game_update_full (false);
  return 1;
}

void
game_update (void)
{
  game_update_full (true);
}
