#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.h"

#include "bffile.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfwindows.h"
#include "bfsvaribl.h"
#include "sb16.h"
#include "sndtimer.h"
#include "snderr.h"
#include "streamfx.h"
#include "aila.h"
#include "ailss.h"
#include "awe32.h"

#include "applog.h"
#include "game_data.h"
#include "util.h"

#pragma pack(1)

extern ubyte byte_5BBE8;
extern ubyte byte_5BBE9;

short startscr_samplevol;
short startscr_midivol;
short startscr_cdvolume;

#pragma pack()

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
    if (AIL_VOC_playback_status(DIGhdriver) == 3)
      byte_5BBE8 = 0;
#else
//TODO
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
#endif
        p_status->field_0 = 0;
    }
}
