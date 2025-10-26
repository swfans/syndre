#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "bfdata.h"
#include "bffile.h"
#include "bflog.h"
#include "bfmemory.h"
#include "bfmouse.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfsound.h"

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

extern ushort sndcard_irq;
extern ushort sndcard_dma;
extern ushort sndcard_ioaddr;
extern ubyte unused_option_p;
extern char Network__Name[18];
extern ubyte DrawFlags;

// From bfsoundlib
extern TbBool SoundAble;
extern TbBool MusicAble;

extern ubyte language;
extern ubyte byte_60B4C;
extern ushort current_levno;
extern ushort level__MapNumber;
extern ubyte is_multiplayer_game;
extern ubyte byte_60B42;
extern ubyte byte_60B44;
extern ubyte byte_60B3B;
extern ubyte byte_60B3A;
extern ubyte byte_60B4C;
extern ubyte byte_60B47;
extern ubyte cheat_credits;
extern ubyte cheat_worldmap;
extern int debug_k;
extern ubyte byte_60B51;
extern ubyte cheats_speedup;
extern ubyte cheats_mission;
extern ushort MouseSwap;
extern struct TbSprite *mouse_sprite;

extern struct TbSprite *pointer_sprites;
extern struct TbSprite *pointer_sprites_end;
extern ubyte *pointer_data;
extern struct TbLoadFiles load_files_vres16[];
extern struct TbLoadFiles load_files_mcga[];

// ASM imports with matching call convention
int * _cdecl ApSpriteSetup_ForceHeight(struct TbSprite *p_start, struct TbSprite *p_end, ubyte *data);
int _cdecl init_sound(ushort sc_irq, ushort sc_dma, ushort sc_ioaddr);
int _cdecl InitMIDI(const char *fname1, char *fname2, ushort sc_irq, ushort sc_dma, ushort sc_ioaddr);
void _cdecl reset_input(void);
void _cdecl ShutdownMIDI(void);
void _cdecl syndicate(void);

/*

int *ApSpriteSetup_ForceHeight(struct TbSprite *p_start, struct TbSprite *p_end, ubyte *data)
{
}

int init_sound(ushort sc_irq, ushort sc_dma, ushort sc_ioaddr)
{
}

int InitMIDI(const char *fname1, char *fname2, ushort sc_irq, ushort sc_dma, ushort sc_ioaddr)
{
}

void reset_input(void)
{
    asm volatile ("call ASM_reset_input\n"
      :  : );
}

void ShutdownMIDI(void)
{
    asm volatile ("call ASM_ShutdownMIDI\n"
      :  : );
}

void syndicate(void)
{
    asm volatile ("call ASM_syndicate\n"
      :  : );
}
*/

static void
run_intro (int argc, char **argv)
{
    char intro_cmdline[100];
    int i;

    strcpy(intro_cmdline, "intro >nul:");

    for (i = 1; i < argc; i++)
    {
        strcat(intro_cmdline, " ");
        strcat(intro_cmdline, argv[i]);
    }

    system(intro_cmdline);
}

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
    {"iirq",        0, NULL, 'R'},
    {"idma",        0, NULL, 'D'},
    {"iio",         0, NULL, 'I'},
    {"nport",       0, NULL, 'p'},
    {"nname",       0, NULL, 'n'},
    {"colang",      0, NULL, 'c'},
    {"lores",       0, NULL, 'L'},
    {"hires",       0, NULL, 'H'},
    {"dunkn",       0, NULL, 'd'},
    {"nosound",     0, NULL, 's'},
    {"windowed",    0, NULL, 'w'},
    {"no-stretch",  0, NULL, 'S'},
    {"help",	    0, NULL, 'h'},
    {NULL,          0, NULL,  0 },
  };

  argv0 = (*argv)[0];
  index = 0;

  while ((val = getopt_long (*argc, *argv, "R:D:I:p:n:c:LHdswSh", options, &index)) >= 0)
    {
      switch (val)
      {
      case 'R':
          sndcard_irq = atoi(optarg);
          break;

      case 'D':
          sndcard_dma = atoi(optarg);
          break;

      case 'I':
          sndcard_ioaddr = atoi(optarg);
          break;

      case 'p':
          unused_option_p = atoi(optarg);
          if (unused_option_p > 16)
              unused_option_p = 16;
          break;

      case 'n':
          strncpy(Network__Name, optarg, sizeof(Network__Name));
          break;

      case 'c':
          language = atoi(optarg);
          if (language >= 3)
              language = 0;
          break;

      case 'L':
          DrawFlags = DrwF_Unkn04;
          break;

      case 'H':
          DrawFlags = DrwF_ScreenVres16;
          break;

      case 'd':
          byte_60B4C = 1;
          break;

      case 's':
          SoundAble = 0;
          MusicAble = 0;
          break;

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

void
nullsub_2 (void)
{
}

void
nullsub_3 (void)
{
}

int main (int argc, char **argv)
{
    run_intro(argc, argv);

    current_levno = 1;
    level__MapNumber = 1;
    DrawFlags = DrwF_ScreenVres16;
    is_multiplayer_game = 0;
    byte_60B42 = 0;
    byte_60B44 = 0;
    byte_60B3B = 0;
    byte_60B3A = 1;
    byte_60B4C = 0;
    byte_60B47 = 1;
    cheat_credits = 0;
    SoundAble = 1;
    MusicAble = 1;
    cheat_worldmap = 0;
    debug_k = 0;
    byte_60B51 = 0;
    language = 0;
    unused_option_p = 0;
    cheats_speedup = 0;
    cheats_mission = 0;

    if (LbErrorLogSetup(NULL, NULL, Lb_ERROR_LOG_NEW) != Lb_SUCCESS)
            printf("Execution log setup failed\n");

    process_options(&argc, &argv);

    printf("SyndicateRE "VERSION"\n"
      "The original by Bullfrog. Port solution by Unavowed and Gynvael Coldwind.\n"
      "Refactored port base by Mefistotelis.\n");

    if (!game_initialise())
        return 1;

    display_set_full_screen(cmdln_fullscreen);
    display_set_lowres_stretch(cmdln_lores_stretch);

#if 0
    int retval;
    // Call game main
    asm volatile ("call ASM_main\n"
      : "=a" (retval) : "a" (argc), "d" (argv));
#else
    if ((DrawFlags & DrwF_ScreenVres16) != 0)
    {
        LbDataLoadAll(load_files_vres16);
    }
    else if ((DrawFlags & DrwF_Unkn04) != 0)
    {
        byte_60B42 = 0;
        LbDataLoadAll(load_files_mcga);
    }
    ApSpriteSetup_ForceHeight(pointer_sprites, pointer_sprites_end, pointer_data);
    mouse_sprite = &pointer_sprites[1];
    MouseSwap = 1;

    LbMouseSetup(mouse_sprite, 256, 256);

    nullsub_3();
# if 1
    InitSound();
# else
    if (SoundAble)
        SoundAble = init_sound(sndcard_irq, sndcard_dma, sndcard_ioaddr);
# endif
    if (MusicAble)
        MusicAble = InitMIDI("data/syngame.xmi", "data/gamefm.dll", sndcard_irq, sndcard_dma, sndcard_ioaddr);

    if (byte_60B42)
    {
        nullsub_2();
    }
    else
    {
        TbClockMSec tmend;

        //TODO OpenIKeyboard_0();
        syndicate();

        tmend = LbTimerClock() + 72 * 5000/91; // 1 from int08 timer is 1000/18.2 miliseconds
        LbSleepUntil(tmend);

        //TODO CloseIKeyboard();
    }
    reset_input();
    LbDataFreeAll(load_files_vres16);
    ShutdownMIDI();
# if 0 // there is broader FreeAudio() call within game_quit()
    FreeSound();
# endif
#endif

    LbErrorLogReset();
    LbMemoryReset();
    game_quit();

    // the above function never returns
    return 0;
}
