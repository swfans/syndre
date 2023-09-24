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
#include "drv_oal.h"
#include "oggvorbis.h"
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

extern char FullDIG_INIPath[144];
extern char FullMDI_INIPath[144];
extern char SoundDataPath[144];
extern char SoundDriverPath[144];

extern AIL_INI MusicInstallChoice;
extern char full_music_data_path[144];
extern char MusicType[6];
extern TbBool DisableLoadMusic;
extern TbBool DisableDangerMusic;
extern TbBool MusicInstalled;
extern TbBool MusicAble;
extern TbBool MusicActive;

extern AIL_INI SoundInstallChoice;
extern ushort SoundType;
extern TbBool DisableLoadSounds;
extern TbBool SoundAble;

extern TbBool CDAble;

extern TbBool UseMultiMediaExtensions;
extern TbBool AutoScanForSoundHardware;
extern TbBool StereoSound;
extern TbBool ive_got_an_sb16;
extern ulong MaxNumberOfSamples;
extern TbBool AILStartupAlreadyInitiated;

extern OggVorbisStream  sound_music_stream;
extern MDI_DRIVER *MusicDriver;
extern DIG_DRIVER *SoundDriver;


//TODO better name?
void fill_ail_sample_ids(void)
{
#if 0
    asm volatile ("call ASM_fill_ail_sample_ids\n"
        :  :  : "eax" );
#endif
}

ubyte load_music_bank(TbFileHandle fh, ubyte bankId)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_load_music_bank\n"
        : "=r" (ret) : "a" (fh),  "d" (bankId));
    return ret;
#endif
}

int LoadMusic(ushort bankNo)
{
#if 0
    int ret;
    asm volatile ("call ASM_LoadMusic\n"
        : "=r" (ret) : "a" (a1));
    return ret;
#endif
    TbFileHandle fh;
    long fsize;
    ulong nbanks_offs;
    ushort nbanks[4];
    ubyte bankId;

    sprintf(SoundProgressMessage, "BF48 - load music bank %d\n", bankNo);
    SoundProgressLog(SoundProgressMessage);

    if (!MusicInstalled) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - music not installed\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (!MusicAble) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - MusicAble = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }
    if (DisableLoadMusic) {
        sprintf(SoundProgressMessage, "BF53 - load music bank - failed - LoadMusic Disabled = 0\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    StopMusic();
    fh = LbFileOpen(full_music_data_path, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE) {
        sprintf(SoundProgressMessage, "BF52 - load music bank - failed - no music.dat\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileSeek(fh, 0, Lb_FILE_SEEK_END);
    fsize = LbFilePosition(fh);

    LbFileSeek(fh, fsize - 4, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, &nbanks_offs, 4);
    LbFileSeek(fh, nbanks_offs, Lb_FILE_SEEK_BEGINNING);
    LbFileRead(fh, nbanks, 8);

    switch (MusicType[0])
    {
    case 'G':
    case 'g':
    default:
        bankId = 0;
        break;
    case 'R':
    case 'r':
        bankId = 1;
        break;
    case 'F':
    case 'f':
        bankId = 2;
        break;
    case 'W':
    case 'w':
        bankId = 3;
        break;
    }

    if (bankNo + 1 > nbanks[bankId]) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF49 - load music bank - failed - bank not found\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileSeek(fh, bankNo << 6, 1u);
    if (!load_music_bank(fh, bankId)) {
        LbFileClose(fh);
        sprintf(SoundProgressMessage, "BF50 - load music bank - failed - cannot allocate\n");
        SoundProgressLog(SoundProgressMessage);
        return 1;
    }

    LbFileClose(fh);
    sprintf(SoundProgressMessage, "BF51 - load music bank - passed\n");
    SoundProgressLog(SoundProgressMessage);
    return 0;
}
