/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file sound.c
 *     Sound related routines.
 * @par Purpose:
 *     Implements routines which help adjust functions from bfsoundlib to
 *     the games needs.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 02 Nov 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.h"

#include "bffile.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfwindows.h"
#include "bfsvaribl.h"
#include "dllload.h"
#include "sb16.h"
#include "sndtimer.h"
#include "snderr.h"
#include "ssampply.h"
#include "streamfx.h"
#include "aila.h"
#include "aildebug.h"
#include "ailss.h"
#include "awe32.h"

#include "applog.h"
#include "game_data.h"
#include "util.h"

/******************************************************************************/
#pragma pack(1)

extern ubyte byte_5BBE8;
extern ubyte byte_5BBE9;
extern sbyte SongCurrentlyPlaying;

extern void *MUSdrvr;
extern struct SNDSEQUENCE *sSOSTrackMap[8];

short startscr_samplevol;
short startscr_midivol;
short startscr_cdvolume;

#pragma pack()
/******************************************************************************/

void sound_bank_setup(void)
{
    struct BFSample *p_smp;

    if (smptable == NULL) {
        LOGERR("Cannot prepare - samples not loaded");
        return;
    }

    if (!GetSoundAble()) {
        LOGERR("Cannot prepare - no sound ability");
        return;
    }

    for (p_smp = &smptable[1]; p_smp < smptable_end; p_smp++)
    {
        void *voc_data;

        voc_data = smpdata + (uint)p_smp->data_start;
        p_smp->data_shifted = ((uint)voc_data) << 12;
        p_smp->data_start = voc_data;
        // Old versions of AIL require pre-formatting samples
#if defined(WITH_AIL2)
        AIL_format_VOC_file(DIGhdriver, voc_data);
#endif
    }
    LOGSYNC("Prepared %d samples", p_smp - smptable - 1);
}

void ClearBFSampleStatus(void)
{
    short smp_id;

    for (smp_id = 0; smp_id < 256; smp_id++)
    {
        struct BFSampleStatus *p_status;

        p_status = &sample_status[smp_id];

        p_status->field_0 = 0;
        p_status->field_1 = 0;
    }
}

void SetBFSampleStatus(ubyte smp_id, ubyte stat)
{
    struct BFSample *p_smp;
    struct BFSampleStatus *p_status;

    p_smp = &smptable[smp_id];
    p_status = &sample_status[smp_id];

    if (p_smp->field_1E == 0)
        return;

    if (p_status->field_0 == 0)
    {
        p_status->field_1 = stat;
        p_status->field_0 = 1;
    }
    else if (stat > p_status->field_1)
    {
        p_status->field_1 = stat;
        p_status->field_0 = 1;
    }
}

void BFPlaySample(ubyte smp_id)
{
    struct BFSample *p_smp;

    if (!GetSoundAble()) {
        LOGERR("Cannot play sample %d - no sound ability", (int)smp_id);
        return;
    }
    if (!GetSoundActive()) {
        LOGERR("Cannot play sample %d - sound not active", (int)smp_id);
        return;
    }
    p_smp = &smptable[smp_id];
    // Old versions of AIL have separate API for VOCs
#if defined(WITH_AIL2)
    AIL_play_VOC_file(DIGhdriver, p_smp->data_start);
    AIL_start_digital_playback(DIGhdriver);
#else
    PlaySampleFromAddress(0, smp_id, 127, 64, 100, 0, 1, p_smp->data_start);
#endif
}

void BFSonundUnkn1(void)
{
    short smp_id, sel_smp_id;

    if (!GetSoundAble()) {
        LOGNO("Cannot update - no sound ability");
        return;
    }
    if (!GetSoundActive()) {
        LOGNO("Cannot update - sound not active");
        return;
    }

    sel_smp_id = 0;
#if defined(WITH_AIL2)
    if (AIL_VOC_playback_status(DIGhdriver) == 3) // 3=DAC_DONE
        byte_5BBE8 = 0;
#else
    byte_5BBE8 = 0; // No need to prioritize a single sample, we now have a mixer
#endif

    for (smp_id = 1; smp_id < 256; smp_id++)
    {
        struct BFSample *p_smp;
        struct BFSampleStatus *p_status;
        ubyte v6;

        p_smp = &smptable[smp_id];
        p_status = &sample_status[smp_id];

        if (p_status->field_0 == 0)
            continue;
        p_status->field_0--;

        v6 = p_smp->field_1E;

        if ((v6 > byte_5BBE8) || (v6 == byte_5BBE8 && !p_smp->field_1F) ||
          (p_smp->field_1E == byte_5BBE8 && p_status->field_1 > byte_5BBE9))
        {
            sel_smp_id = smp_id;
            byte_5BBE8 = p_smp->field_1E;
            byte_5BBE9 = p_status->field_1;
        }
    }

    if (sel_smp_id != 0)
    {
        struct BFSample *p_smp;
        struct BFSampleStatus *p_status;

        p_smp = &smptable[sel_smp_id];
        p_status = &sample_status[sel_smp_id];

    // Old versions of AIL have separate API for VOCs
#if defined(WITH_AIL2)
        AIL_play_VOC_file(DIGhdriver, p_smp->data_start);
        AIL_start_digital_playback(DIGhdriver);
#else
        PlaySampleFromAddress(0, sel_smp_id, 127, 64, 100, 0, 1, p_smp->data_start);
#endif
        p_status->field_0 = 0;
    }
}

void BFSoundPause(void)
{
    if (!GetSoundAble()) {
        LOGNO("Cannot pause - no sound ability");
        return;
    }
    if (!GetSoundActive()) {
        LOGNO("Cannot pause - sound not active");
        return;
    }

#if defined(WITH_AIL2)
    AIL_pause_digital_playback(DIGhdriver);
#else
    PauseAllSamples();
#endif
}

void BFSoundResume(void)
{
    if (!GetSoundAble()) {
        LOGNO("Cannot resume - no sound ability");
        return;
    }
    if (!GetSoundActive()) {
        LOGNO("Cannot resume - sound not active");
        return;
    }

#if defined(WITH_AIL2)
    AIL_resume_digital_playback(DIGhdriver);
#else
    ResumeAllSamples();
#endif
}

int InitMIDI(const char *bank_fname, char *drv_fname,
  ushort sc_irq, ushort sc_dma, ushort sc_ioaddr)
{
    void *p_musbank;
#if defined(WITH_AIL2)
    char GTL_filename[96];
    FILE *GTL_fh;
    struct drvr_desc *desc;
    uint sequence_num;
    int sc_drq;
    int state_size;
    ushort tc_size;

    {
        void *drvr_buf;
        void *drvr_base;

        drvr_buf = FILE_read(drv_fname, 0);
        if (drvr_buf == NULL) {
            LOGERR("Cannot read driver - %s", strerror(errno));
            return 0;
        }
        drvr_base = DLL_load(drvr_buf, 5u, 0);
        if (drvr_base == NULL) {
            LOGERR("Cannot load driver DLL");
            return 0;
        }
        free(drvr_buf);

        MUSdrvr = AIL_register_driver(drvr_base);
        if (MUSdrvr == (void *)-1) {
            LOGERR("Cannot register driver");
            return 0;
        }
    }

    desc = AIL_describe_driver(MUSdrvr);
    if (desc->drvr_type != 3) {
        LOGERR("Invalid driver type");
        return 0;
    }
    if (sc_irq == 0)
        sc_irq = desc->default_IRQ;
    if (sc_dma == 0)
        sc_dma = desc->default_DMA;
    if (sc_ioaddr == 0)
        sc_ioaddr = desc->default_IO;
    sc_drq = desc->default_DRQ;
    if (!AIL_detect_device(MUSdrvr, sc_ioaddr, sc_irq, sc_dma, sc_drq))
        return 0;
    AIL_init_driver(MUSdrvr, sc_ioaddr, sc_irq, sc_dma, desc->default_DRQ);
    state_size = AIL_state_table_size(MUSdrvr);

    p_musbank = FILE_read(bank_fname, 0);
    if (p_musbank == NULL) {
        LOGERR("Cannot read music bank - %s", strerror(errno));
        return 0;
    }
    strcpy(GTL_filename, "data/sample.");
    strcat(GTL_filename, desc->data_suffix);
    tc_size = AIL_default_timbre_cache_size(MUSdrvr);
    if (tc_size > 0)
    {
        void *p_cache;
        p_cache = malloc(tc_size);
        AIL_define_timbre_cache(MUSdrvr, p_cache, tc_size);
    }

    GTL_fh = fopen(GTL_filename, "rb");
    for (sequence_num = 0; sequence_num < 8; sequence_num++)
    {
        void *p_state;
        void *global_timbre;
        struct SNDSEQUENCE *seq;

        p_state = malloc(state_size);
        seq = AIL_register_sequence(MUSdrvr, p_musbank, sequence_num, p_state, 0);
        sSOSTrackMap[sequence_num] = seq;
        if (seq == (struct SNDSEQUENCE *)-1) {
            free(p_state);
            break;
        }
        while ( 1 )
        {
            ushort req;
            int rq_l, rq_h;

            req = AIL_timbre_request(MUSdrvr, sSOSTrackMap[sequence_num]);
            if (req == 0xFFFF) {
                break;
            }
            rq_l = req % 256;
            rq_h = req / 256;
            global_timbre = load_global_timbre(GTL_fh, rq_h, rq_l);
            if (global_timbre == NULL) {
                LOGERR("Cannot load global timbre, sequence %d request 0x%04x", sequence_num, (uint)req);
                return 0;
            }
            AIL_install_timbre(MUSdrvr, rq_h, rq_l, global_timbre);
            free(global_timbre);
        }
    }
    if (GTL_fh != NULL)
        fclose(GTL_fh);
    return 1;
#else
    p_musbank = FILE_read(bank_fname, 0);
    if (p_musbank == NULL) {
        LOGERR("Cannot read music bank - %s", strerror(errno));
        return 0;
    }
    //TODO unfinished
    return 0;
#endif
}

void BFMidiStartMusic(short song_no)
{
    if (!GetMusicAble()) {
        LOGNO("Cannot start - no music ability");
        return;
    }
    if (!GetMusicActive()) {
        if (SongCurrentlyPlaying >= 0) {
#if defined(WITH_AIL2)
            AIL_stop_sequence(MUSdrvr, sSOSTrackMap[SongCurrentlyPlaying]);
#else
            AIL_stop_sequence(sSOSTrackMap[SongCurrentlyPlaying]);
#endif
            SongCurrentlyPlaying = -1;
        }
        LOGNO("Cannot start - music not active");
        return;
    }
    if (SongCurrentlyPlaying != song_no)
    {
        if (SongCurrentlyPlaying >= 0)
#if defined(WITH_AIL2)
            AIL_stop_sequence(MUSdrvr, sSOSTrackMap[SongCurrentlyPlaying]);
#else
            AIL_stop_sequence(sSOSTrackMap[SongCurrentlyPlaying]);
#endif
        SongCurrentlyPlaying = song_no;
#if defined(WITH_AIL2)
        AIL_start_sequence(MUSdrvr, sSOSTrackMap[song_no]);
#else
        AIL_start_sequence(sSOSTrackMap[song_no]);
#endif
    }
}

/******************************************************************************/
