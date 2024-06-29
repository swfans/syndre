/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file bfaudio.c
 *     Reimplementation of Bullfrog Sound Library.
 * @par Purpose:
 *     Set of general audio-related calls.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Jun 2022 - 05 Sep 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "bfaudio.h"
#include "bfmemory.h"
#include "msssys.h"
#include "aildebug.h"
/******************************************************************************/

TbBool AILStartupAlreadyInitiated = false;
TbBool AutoScanForSoundHardware = true;
char SoundDataPath[144];
char SoundDriverPath[144];

AIL_INI SoundInstallChoice;
char FullDIG_INIPath[144];
TbBool SoundInstalled = false;
TbBool SoundAble = true;
ushort SoundType = 0;
TbBool DisableLoadSounds = false;

ulong MaxNumberOfSamples = 10;
TbBool StereoSound = true;
TbBool UseMultiMediaExtensions = false;
long CurrentSoundMasterVolume = 127;
TbBool ive_got_an_sb16 = false;

AIL_INI MusicInstallChoice;
char FullMDI_INIPath[144];
TbBool MusicAble = true;
TbBool DisableLoadMusic = false;
TbBool DisableDangerMusic = false;

long CurrentMusicMasterVolume = 127;

extern DIG_DRIVER *SoundDriver;
extern MDI_DRIVER *MusicDriver;
extern TbBool MusicInstalled;
extern TbBool StreamedSoundAble;

/******************************************************************************/

void FreeAudio(void)
{
#if 0
    asm volatile ("call ASM_FreeAudio\n"
        :  :  : "eax" );
#endif
}

/** Wrapper for LbMemoryAlloc(), needed to make sure data sizes match.
 */
void *LbMemoryAlloc_wrap(uint32_t size)
{
    return LbMemoryAlloc(size);
}

/** Wrapper for LbMemoryFree(), needed to make sure data sizes match.
 */
void LbMemoryFree_wrap(void *ptr)
{
    LbMemoryFree(ptr);
}

void EnsureAILStartup(void)
{
    if (!AILStartupAlreadyInitiated)
    {
        AIL_MEM_use_malloc(LbMemoryAlloc_wrap);
        AIL_MEM_use_free(LbMemoryFree_wrap);
        AIL_startup();
        AILStartupAlreadyInitiated = 1;
    }
}

void SetSoundMasterVolume(long vol)
{
#if 0
    asm volatile ("call ASM_SetSoundMasterVolume\n"
        :  : "a" (vol));
#endif
    if (!SoundAble || !SoundInstalled)
        return;
    if (vol == CurrentSoundMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_digital_master_volume(SoundDriver, vol);
    CurrentSoundMasterVolume = vol;
}

void SetMusicMasterVolume(long vol)
{
#if 0
    asm volatile ("call ASM_SetMusicMasterVolume\n"
        :  : "a" (vol));
#endif
    if (!MusicAble || !MusicInstalled)
        return;
    if (vol == CurrentMusicMasterVolume || vol > 127 || vol < 0)
        return;
    AIL_set_XMIDI_master_volume(MusicDriver, vol);
    CurrentMusicMasterVolume = vol;
}
/******************************************************************************/
