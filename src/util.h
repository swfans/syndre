/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file util.h
 *     Header file for util.c.
 * @par Purpose:
 *     Simple utilities for general use.
 * @par Comment:
 *     Routines implementing small utilities.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 12 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

#ifdef WIN32
#ifndef strcasecmp
# define strcasecmp stricmp
# define strncasecmp strnicmp
#endif
# define mkdir(path, mode) mkdir (path)
#endif

#if defined(__GNUC__) && !defined(_cdecl)
# define _cdecl __attribute__((__cdecl__))
#endif

/******************************************************************************/

void strtolower (char *string);
void strtocapwords(char *s);

const char *extract_path_segment (const char *path,
				  char *buffer, size_t size);

void *xmalloc (size_t size);
void *xcalloc (size_t size);
void xfree (void *ptr);
char *xstrdup (const char *str);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif // UTIL_H
