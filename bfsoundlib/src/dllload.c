/******************************************************************************/
// Bullfrog Sound Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file dllload.c
 *     OpenAL based reimplementation of MSS API.
 * @par Purpose:
 *     File load functions from MSS API.
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
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "dllload.h"
#include "mssal.h"
#include "msssys.h"
/******************************************************************************/
extern int32_t disk_err;
/******************************************************************************/

int32_t FILE_size(char const *fname)
{
    int fh;
    int32_t len;

    disk_err = AIL_NO_ERROR;
    fh = open(fname, O_RDONLY);
    if (fh == -1) {
        disk_err = AIL_FILE_NOT_FOUND;
        return -1;
    }
    len = lseek(fh, 0, SEEK_END);
    close(fh);

    return len;
}

/******************************************************************************/
