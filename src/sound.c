#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.h"

#include "bffile.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfwindows.h"
#include "bfscd.h"
#include "sb16.h"
#include "sndtimer.h"
#include "snderr.h"
#include "streamfx.h"
#include "aila.h"
#include "ailss.h"
#include "awe32.h"

#include "game_data.h"
#include "util.h"

#pragma pack(1)

short startscr_samplevol;
short startscr_midivol;
short startscr_cdvolume;

#pragma pack()

//TODO better name?
void fill_ail_sample_ids(void)
{
#if 0
    asm volatile ("call ASM_fill_ail_sample_ids\n"
        :  :  : "eax" );
#endif
}
