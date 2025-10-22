/******************************************************************************/
// BullfrogGame Port, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file applog.h
 *     Application debug logging macros.
 * @par Purpose:
 *     Macros to simplify logging within the application.
 * @par Comment:
 *     Any libraries should have its own collection of macros.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef APPLOG_H_
#define APPLOG_H_

#include "bflog.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/

/** Macro for logging messages of error severity within this module. */
#define LOGERR(format,args...) LbErrorLog("SyndRE: %s: " format "\n", __func__ , ## args)

/** Macro for logging messages of warning severity within this module. */
#define LOGWARN(format,args...) LbWarnLog("SyndRE: %s: " format "\n", __func__ , ## args)

#ifdef DEBUG
#define LOGSYNC(format,args...) LbSyncLog("SyndRE: %s: " format "\n", __func__ , ## args)
#define LOGDBG(format,args...) LbDbgLog("SyndRE: %s: " format "\n", __func__ , ## args)
#else
#define LOGSYNC(format,args...)
#define LOGDBG(format,args...)
#endif

/** Macro for messages which are only for developers and disabled in normal builds. */
#define LOGNO(format,args...)

/******************************************************************************/
#ifdef __cplusplus
};
#endif

#endif // APPLOG_H_
