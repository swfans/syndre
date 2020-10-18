/******************************************************************************/
// Syndicate Wars Port - open-source reimplementation of SW.
/******************************************************************************/
/** @file game_data.c
 *     Memory managing routines.
 * @par Purpose:
 *     Structures and functions defining data used by the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "game_data.h"

#include "bflib_basics.h"
#include "bflib_fileio.h"
#include "unix.h"
#include "windows.h"
#include "dos.h"

static char data_path_user[DISKPATH_SIZE] = "";
static char data_path_hdd[DISKPATH_SIZE] = "";
static char game_dir_savegame[] = "qdata" FS_SEP_STR "savegame";

/******************************************************************************/

const char *
GetDirectoryUser(void)
{
    if (data_path_user[0] == '\0')
    {
        if (!sys_get_user_path(data_path_user, sizeof(data_path_user)))
        {
            snprintf(data_path_user, sizeof(data_path_user), ".");
        }
        DEBUGLOG(0,"Dir for user files '%s'\n", data_path_user);
        char path_create[DISKPATH_SIZE];
        snprintf(path_create, sizeof(path_create), "%s" FS_SEP_STR "%s", data_path_user, game_dir_savegame);
        LbDirectoryMake(path_create, true);
    }
    return data_path_user;
}

const char *
GetDirectoryHdd(void)
{
    if (data_path_hdd[0] == '\0')
    {
        if (!sys_get_data_path(data_path_hdd, sizeof(data_path_hdd)))
        {
            snprintf(data_path_hdd, sizeof(data_path_hdd), "%s", ".");
        }
        DEBUGLOG(0,"Dir with HDD data '%s'\n",data_path_hdd);
    }
    return data_path_hdd;
}


void BfGameFileNameTransform(char *out_fname, const char *inp_fname)
{
    char transformed[DISKPATH_SIZE];
    const char *base_dir;

    base_dir = GetDirectoryHdd();

    // Add base path only if the input one is not absolute
    if (inp_fname[0] == '\\' || inp_fname[0] == '/'
      || (strlen (inp_fname) >= 2 && inp_fname[1] == ':')) {
        snprintf (transformed, DISKPATH_SIZE, "%s", inp_fname);
    } else {
        snprintf(transformed, DISKPATH_SIZE, "%s" FS_SEP_STR "%s", base_dir, inp_fname);
    }
    dos_path_to_native(transformed, out_fname, DISKPATH_SIZE);
}

void setup_file_names(void)
{
    lbFileNameTransform = BfGameFileNameTransform;
    // This fills the path variable; for user, it also creates the folder
    GetDirectoryHdd();
    GetDirectoryUser();
}
/******************************************************************************/
