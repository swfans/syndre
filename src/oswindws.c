/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file oswindows.h
 *     Implementation of few functions which is specific to Windows OS.
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
#ifdef WIN32

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <stdio.h>
#include <stdlib.h>

#include "oswindws.h"

#define REGISTRY_KEY "Software\\SyndicateRE\\CurrentVersion"
#define REGISTRY_VALUE "InstallPath"

#ifndef HAVE_GETTIMEOFDAY

/******************************************************************************/

int gettimeofday (struct timeval *tp, struct timezone *tzp)
{
    SYSTEMTIME st;

    /* timezone is obsolete according to UNIX man pages */
    (void) tzp;

    /* Set time */
    tp->tv_sec = (long) time (NULL);

    /* Get time info from the system */
    GetLocalTime (&st);
    tp->tv_usec = (long) st.wMilliseconds * 1000;

    /* Anti race condition sec fix
     * When the milliseconds are at 999 at the time of call to time(), and at
     * 999+1 = 0 at the time of the GetLocalTime call, then the tv_sec and
     * tv_usec would be set to one second in the past. To correct this, just
     * check if the last decimal digit of the seconds match, and if not, add
     * a second to the tv_sec.
     * See below for a race condition PoC (after 5-10 seconds it breaks the
     * loop if the below is commented out)
     */
    if (tp->tv_sec % 10 != st.wSecond % 10)
        tp->tv_sec++;

    /* return success, nothing there to fail */
    return 0;
}
#endif

bool sys_read_string_registry_key(HKEY top_key, const char *key_name,
  const char *value_name, char *buffer, size_t size)
{
    HKEY hkey;
    DWORD type;
    DWORD ksize = size;

    if (RegOpenKeyEx (top_key, key_name, 0, KEY_READ, &hkey)
      != ERROR_SUCCESS)
    {
        return false;
    }

    if (RegQueryValueEx (hkey, value_name, NULL, &type, (LPBYTE) buffer,
      &ksize) != ERROR_SUCCESS)
    {
        RegCloseKey (hkey);
        return false;
    }

    RegCloseKey (hkey);

    if (type != REG_SZ)
        return false;

    return true;
}

bool sys_get_data_path (char *buffer, size_t size)
{
    /* Check HKCU first, we don't want to force Windows users to
     * install the game with administator privileges. */
    if (sys_read_string_registry_key (HKEY_CURRENT_USER, REGISTRY_KEY,
      REGISTRY_VALUE, buffer, size))
    {
        return true;
    }

    /* Fallback to default HKLM setting if HKCU is not found */
    if (sys_read_string_registry_key (HKEY_LOCAL_MACHINE, REGISTRY_KEY,
      REGISTRY_VALUE, buffer, size))
    {
        return true;
    }

    /* Return false */
    return false;
}

bool sys_get_user_path (char *buffer, size_t size)
{
    const char *app_data;

    /* Use Windows folders only if related reg keys are set.
     * If there are no keys, then both functions should refuse
     * to return a folder.
     */
    if (!sys_get_data_path(buffer, size))
        return false;

    app_data = getenv ("APPDATA");

    if (app_data == NULL)
        return false;

    snprintf (buffer, size, "%s\\SyndicateRE", app_data);
    return true;
}

/******************************************************************************/
#endif  // WIN32
