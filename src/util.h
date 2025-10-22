/******************************************************************************/
// SyndicateRE, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file util.h
 *     Header file for buffered handler for keystrokes.
 * @par Purpose:
 *     Taps keyboard event handler and stores each key pressed.
 * @par Comment:
 *     When keys are handled outside the event, it is possible that some
 *     keypresses will not be registered, making typing text frustrating.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 13 Oct 2021
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

/******************************************************************************/

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef WIN32
#ifndef strcasecmp
# define strcasecmp stricmp
# define strncasecmp strnicmp
#endif
# define mkdir(path, mode) mkdir (path)
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
#endif // UTIL_H
