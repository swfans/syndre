/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file osunix.c
 *     Implementation of few functions which is specific to UNIX systems.
 * @par Purpose:
 *     OS-agnostic API for functionalities differently designed for different
 *     OSes.
 * @par Comment:
 *     None.
 * @author   Gynvael Coldwind
 * @author   Unavowed
 * @date     10 Oct 2012 - 13 Oct 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifdef __unix__

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "bffile.h"
#include "osunix.h"
#include "util.h"

/******************************************************************************/

void unix_restore_signal_handlers(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}

bool sys_get_user_path(char *buffer, size_t size)
{
    char *xdg_config_home;
    char *home;

    xdg_config_home = getenv("XDG_CONFIG_HOME");
    if (xdg_config_home != NULL)
    {
        snprintf(buffer, size,
          "%s" FS_SEP_STR "syndicatfx", xdg_config_home);
        return true;
    }

    home = getenv("HOME");
    if (home == NULL)
    {
        fprintf(stderr, "Warning: HOME is not set.\n");
        return false;
    }

#ifndef __APPLE__
    snprintf(buffer, size,
      "%s" FS_SEP_STR ".config" FS_SEP_STR "syndicatfx", home);
#else
    snprintf(buffer, size,
      "%s/Library/Preferences/SyndicatFX", home);
#endif

    return true;
}

bool sys_get_data_path(char *buffer, size_t size)
{
    snprintf(buffer, size, GAME_DATA_PATH);
    return true;
}

/******************************************************************************/
#endif // __unix__
