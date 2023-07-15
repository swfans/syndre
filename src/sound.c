#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include OPENAL_ALC_H
#include OPENAL_AL_H

#include "sound.h"

#include "bffile.h"
#include "bfsound.h"
#include "bfwindows.h"
#include "bfscd.h"
#include "drv_oal.h"
#include "oggvorbis.h"
#include "snderr.h"
#include "ailss.h"

#include "game_data.h"
#include "util.h"


#define SOUND_MAX_SOURCES     64
#define SOUND_BUFFERS_PER_SRC 3
#define SOUND_MAX_BUFFERS     (SOUND_MAX_SOURCES * SOUND_BUFFERS_PER_SRC \
  + SOUND_MUSIC_BUFFERS)
#define SOUND_MAX_BUFSIZE     2048


#define check_al(source) check_al_line((source), __LINE__)
bool check_al_line(const char *source, int line);


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
extern TbBool UseCurrentAwe32Soundfont;
extern TbBool ive_got_an_sb16;
extern ulong MaxNumberOfSamples;

size_t sound_free_buffer_count = 0;
static ALuint        sound_free_buffers[SOUND_MAX_BUFFERS];
extern OggVorbisStream  sound_music_stream;

extern MDI_DRIVER *MusicDriver;
extern DIG_DRIVER *SoundDriver;


#define check_alc(source) check_alc_line ((source), __LINE__)
bool check_alc_line(const char *source, int line);

static ALuint
pop_free_buffer (void)
{
  assert (sound_free_buffer_count > 0);
  return sound_free_buffers[--sound_free_buffer_count];
}

static void
push_free_buffer (ALuint buf)
{
  assert (sound_free_buffer_count < SOUND_MAX_BUFFERS);
  sound_free_buffers[sound_free_buffer_count++] = buf;
}

static bool create_sources(void)
{
  size_t n, m;
  ALuint buffers[SOUND_BUFFERS_PER_SRC];

  for (n = 0; n < SOUND_MUSIC_BUFFERS; n++)
    {
      alGenBuffers (1, &buffers[0]);
      if (!check_al ("alGenBuffers"))
        goto err;

      push_free_buffer (buffers[0]);
    }

  for (n = 0; n < SOUND_MAX_SOURCES; n++)
    {
      alGenBuffers (SOUND_BUFFERS_PER_SRC, buffers);
      if (alGetError () != AL_NO_ERROR)
        {
          n--;
          break;
        }

      for (m = 0; m < SOUND_BUFFERS_PER_SRC; m++)
        push_free_buffer (buffers[m]);
    }

#ifdef DEBUG
    printf ("OpenAL: Created %zu sound buffers.\n", sound_free_buffer_count);
#endif

  return true;

err:
  /* TODO: clean up */
  return false;
}

int oal_sound_initialise(void)
{
  if (!create_sources ())
    goto err;

  if (LbRegisterIdleHandler(sound_update) != Lb_SUCCESS)
    goto err;

  return true;

err:
  return false;
}

void InitAudio(AudioInitOptions *audOpts)
{
#if 0
    asm volatile (
      "call ASM_InitAudio\n"
        : : "a" (audOpts));
#endif
    if (getenv("DEBUG_BF_AUDIO"))
        DebugAudio = 1;

    sprintf(FullDIG_INIPath, "sound/DIG.INI");
    sprintf(FullMDI_INIPath, "sound/MDI.INI");
    strcpy(SoundDataPath, "sound");
    strcpy(SoundDriverPath, "sound");
    if (audOpts->IniPath) {
        sprintf(FullDIG_INIPath, "%s/DIG.INI", audOpts->IniPath);
        sprintf(FullMDI_INIPath, "%s/MDI.INI", audOpts->IniPath);
    }
    if (audOpts->SoundDataPath != NULL) {
        strcpy(SoundDataPath, audOpts->SoundDataPath);
    }
    if (audOpts->SoundDriverPath != NULL) {
        strcpy(SoundDriverPath, audOpts->SoundDriverPath);
    }

    MaxNumberOfSamples = audOpts->MaxSamples;
    SoundType = audOpts->SoundType;
    if (!audOpts->AutoScan)
        AutoScanForSoundHardware = 0;
    if (!audOpts->StereoOption)
        StereoSound = 0;
    if (audOpts->DisableDangerMusic == 1)
        DisableDangerMusic = 1;
    if (audOpts->DisableLoadSounds == 1)
        DisableLoadSounds = 1;
    if (audOpts->DisableLoadMusic == 1)
        DisableLoadMusic = 1;
    if (audOpts->UseCurrentAwe32Soundfont == 1)
        UseCurrentAwe32Soundfont = 1;
    if (audOpts->UseMultiMediaExtensions == 1)
        UseMultiMediaExtensions = 1;
    sprintf(SoundProgressMessage, "BF1  - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF2  - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    if (!(audOpts->AbleFlags & 0x01))
        MusicAble = 0;
    if (!(audOpts->AbleFlags & 0x02))
        SoundAble = 0;

    InitSound();
    InitMusic();

    if (audOpts->InitRedbookAudio == 1) {
        InitRedbook();
    } else if (audOpts->InitRedbookAudio == 2) {
        char mdir[FILENAME_MAX];
        snprintf(mdir, sizeof(mdir),
              "%s" FS_SEP_STR "music", GetDirectoryHdd());
        InitMusicOGG(mdir);
    } else {
        sprintf(SoundProgressMessage, "BF101 - cd init - disabled\n");
        SoundProgressLog(SoundProgressMessage);
        CDAble = false;
    }

    sprintf(SoundProgressMessage, "BF54 - MA   %d\n", MusicAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF55 - SA   %d\n", SoundAble);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF56 - music driver = %s\n", MusicInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
    sprintf(SoundProgressMessage, "BF57 - sound driver = %s\n", SoundInstallChoice.driver_name);
    SoundProgressLog(SoundProgressMessage);
}

static void
destroy_sources(DIG_DRIVER *digdrv)
{
    alDeleteBuffers (sound_free_buffer_count, sound_free_buffers);
    check_al ("alDeleteBuffers");
}

void oal_sound_finalise(void)
{
    destroy_sources(SoundDriver);
}

static ALenum
get_pcm_format(SNDSAMPLE *s)
{
    switch (s->format)
    {
    case 0: return AL_FORMAT_MONO8;
    case 1: return AL_FORMAT_MONO16;
    case 2: return AL_FORMAT_STEREO8;
    case 3: return AL_FORMAT_STEREO16;
    default:
      abort ();
    }
}

static void
queue_source_buffers(DIG_DRIVER *digdrv, SNDSAMPLE *s)
{
    size_t len, total_len;
    size_t buffers_used;
    ALuint source;
    void *data;
    float x_pos;
    ALint state;
    ALuint buf = 0;

    source = s->system_data[5];
    buffers_used = s->system_data[4];

    if (buffers_used >= SOUND_BUFFERS_PER_SRC)
        return;

    total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];

    if (total_len == 0)
    {
        if (s->done[s->current_buffer ^ 1] == 0)
            s->current_buffer ^= 1;
        else
            return;

        total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
    }


    while (total_len > 0 && buffers_used < SOUND_BUFFERS_PER_SRC)
    {
        data = s->start[s->current_buffer] + s->pos[s->current_buffer];
        len = MIN (total_len, SOUND_MAX_BUFSIZE);

        assert ((s->flags & 1) != 0);

        buf = pop_free_buffer ();
        alBufferData (buf, get_pcm_format (s), data, len, s->playback_rate);
        if (!check_al ("alBufferData"))
            goto err;

        alSourceQueueBuffers(source, 1, &buf);
        if (!check_al ("alSourceQueueBuffers"))
            goto err;

        buffers_used++;

        alGetSourcei (source, AL_SOURCE_STATE, &state);
        if (!check_al ("alGetSourcei (AL_SOURCE_STATE)"))
            goto err;

        alSourcef (source, AL_GAIN,
             (s->volume * (1.f / 127.f)
              * (digdrv->master_volume * (1.f / 127.f))));
      if (!check_al ("alSourcef (AL_GAIN)"))
        goto err;

      /* XXX: check if panning/position is OK */
      if (s->pan == 0)
        x_pos = 0.f;
      else
        x_pos = (127 - s->pan - 64) * (1.f / 64.f);

      alSource3f (src->name, AL_POSITION, x_pos, 0.f, -.25f);
      if (!check_al ("alSource3f (AL_POSITION)"))
        goto err;

      if (state != AL_PLAYING)
        {
          alSourcePlay (src->name);
          if (!check_al ("alSourcePlay"))
            goto err;
        }

      s->pos[s->current_buffer] += len;
      total_len -= len;

      if (total_len == 0 && s->loop_count == 0)
        {
          s->pos[s->current_buffer] = 0;
          total_len = s->len[s->current_buffer] - s->pos[s->current_buffer];
        }
    }

  return;

err:
  if (buf != 0)
    push_free_buffer (buf);
}

static void
unqueue_source_buffers(SNDSAMPLE *s)
{
    size_t buffers_used;
    ALuint source;

    source = s->system_data[5];
    buffers_used = s->system_data[4];

    if (buffers_used == 0)
        return;

    buffers_used -=
        OPENAL_unqueue_source_buffers(source,
                   (SoundNameCallback) push_free_buffer,
                   NULL);

    s->system_data[4] = buffers_used;

    if (buffers_used > 0
        || s->pos[s->current_buffer] < s->len[s->current_buffer]
        || s->loop_count == 0)
      return;

    /* this check prevents the sound in the intro from abruptly stopping */
    if (!s->done[0] && !s->done[1])
        return;

    alSourceStop(source);
    check_al ("alSourceStop");
    s->status = 2;
}

void sound_update_dig_samples(DIG_DRIVER *digdrv)
{
    int32_t n;

    digdrv->n_active_samples = 0;

    for (n = 0; n < digdrv->n_samples; n++)
    {
        SNDSAMPLE *s;

        s = &digdrv->samples[n];

        if (s->status == 1)
            continue;

#if 0
        printf ("sample %i loops:%i ([%i/%i] [%i/%i]) (%i %i) %zu\n", n,
              s->loop_count,
              s->pos[0], s->len[0],
              s->pos[1], s->len[1],
              s->done[0], s->done[1],
              buffers_used);
#endif

        unqueue_source_buffers(s);

        if (s->status != 4)
            continue;

        digdrv->n_active_samples++;

        queue_source_buffers(digdrv, s);
    }
}

void sound_update_mdi_sequences(MDI_DRIVER *mdidrv)
{
    int32_t i;

    for (i = 0; i < mdidrv->n_sequences; i++)
    {
        SNDSEQUENCE *seq;

        seq = &mdidrv->sequences[i];

        if (seq->status != SNDSEQ_PLAYING)
            continue;

        //TODO play MIDI

    }
}

TbBool sound_update(void)
{
    if (!SoundDriver)
        return false;
    if (!SoundDriver->drvr->initialized)
        return false;

    if (SoundDriver != NULL)
        sound_update_dig_samples(SoundDriver);
    if (MusicDriver != NULL)
        sound_update_mdi_sequences(MusicDriver);

    ogg_vorbis_stream_update(&sound_music_stream);
    return true;
}

void StopMusicIfActive(void)
{
    if (!MusicInstalled || !MusicAble)
        return;
    if (MusicActive)
    {
        StopMusic();
        MusicActive = 0;
    }
}

void StopMusic(void)
{
#if 0
    asm volatile ("call ASM_StopMusic\n"
        :  :  : "eax" );
#endif
}

//TODO better name?
void fill_ail_sample_ids(void)
{
#if 0
    asm volatile ("call ASM_fill_ail_sample_ids\n"
        :  :  : "eax" );
#endif
}

int LoadSounds(unsigned char a1)
{
#if 0
    int ret;
    asm volatile ("call ASM_LoadSounds\n"
        : "=r" (ret) : "a" (a1));
    return ret;
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
