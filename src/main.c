#include <SDL.h>
#include <getopt.h>
#include <stdlib.h>

#include "bffile.h"
#include "bfmemory.h"
#include "bfscreen.h"
#include "display.h"
#include "game.h"
#include "game_data.h"
#include "util.h"

#if defined WIN32 && defined main
// Anti SDL
# undef main
#endif

TbBool cmdln_fullscreen = true;
TbBool cmdln_lores_stretch = true;

static void
print_help (const char *argv0)
{
  printf (
"Usage: %s [OPTIONS]\n\n"
"Available options:\n"
"  --windowed    -w    Run in windowed mode\n"
"  --no-stretch  -S    Don't display 320x200 graphics stretched to 640x480\n"
"  --help        -h    Display the help message\n",
  argv0);
}

static void
process_options (int *argc, char ***argv)
{
  int index;
  int val;
  char *argv0;

  static struct option options[] =
  {
    {"windowed",    0, NULL, 'w'},
    {"no-stretch",  0, NULL, 'S'},
    {"help",	    0, NULL, 'h'},
    {NULL,          0, NULL,  0 },
  };

  argv0 = (*argv)[0];
  index = 0;

  while ((val = getopt_long (*argc, *argv, "wSh", options, &index)) >= 0)
    {
      switch (val)
      {
      case 'w':
          cmdln_fullscreen = false;
          break;

      case 'S':
          cmdln_lores_stretch = false;
          break;

      case 'h':
          print_help (argv0);
          exit (0);
          break;

      default:
          exit (1);
      }
    }

  *argc -= optind - 1;
  *argv += optind - 1;

  (*argv)[0] = argv0;
}

int main (int argc, char **argv)
{
    int retval;

    process_options(&argc, &argv);

    printf("BullfrogGame Port "VERSION"\n"
      "The original by Bullfrog. Port solution by Unavowed and Gynvael Coldwind.\n"
      "Refactored port base by Mefistotelis.\n");

    if (!game_initialise())
        return 1;

    // Call game main
    asm volatile ("call ASM_main\n"
      : "=a" (retval) : "a" (argc), "d" (argv));

    game_quit();

    // the above function never returns
    return 0;
}
