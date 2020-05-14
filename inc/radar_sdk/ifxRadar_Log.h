/* ===========================================================================
** Copyright (C) 2019 Infineon Technologies AG. All rights reserved.
** ===========================================================================
**
** ===========================================================================
** Infineon Technologies AG (INFINEON) is supplying this file for use
** exclusively with Infineon's sensor products. This file can be freely
** distributed within development tools and software supporting such
** products.
**
** THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
** OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
** INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON
** WHATSOEVER.
** ===========================================================================
*/
/**
 * @file ifxRadar_Log.h
 *
 */

#ifndef IFX_RADAR_LOG_H
#define IFX_RADAR_LOG_H

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifndef IFX_STDOUT
#include <stdio.h>
#define IFX_STDOUT stderr
#endif

typedef enum
{
    IFX_LOG_INFO,
    IFX_LOG_WARNING,
    IFX_LOG_ERROR,
    IFX_LOG_DEBUG
} ifx_Log_Severity_t;

#if defined(IFX_LOG_SEVERITY_DEBUG)
#define IFX_LOG_DEBUG(...) ifx_log(IFX_STDOUT, IFX_LOG_DEBUG, __VA_ARGS__)
#else
#define IFX_LOG_DEBUG(...)
#endif

#if defined(IFX_LOG_SEVERITY_INFO) || \
    defined(IFX_LOG_SEVERITY_DEBUG)
#define IFX_LOG_INFO(...) ifx_log(IFX_STDOUT, IFX_LOG_INFO, __VA_ARGS__)
#else
#define IFX_LOG_INFO(...)
#endif

#if defined(IFX_LOG_SEVERITY_INFO)    || \
    defined(IFX_LOG_SEVERITY_WARNING) || \
    defined(IFX_LOG_SEVERITY_DEBUG)
#define IFX_LOG_WARNING(...) ifx_log(IFX_STDOUT, IFX_LOG_WARNING, __VA_ARGS__)
#else
#define IFX_LOG_WARNING(...)
#endif

#if defined(IFX_LOG_SEVERITY_INFO)    || \
    defined(IFX_LOG_SEVERITY_WARNING) || \
    defined(IFX_LOG_SEVERITY_ERROR)   || \
    defined(IFX_LOG_SEVERITY_DEBUG)
#define IFX_LOG_ERROR(...)   ifx_log(IFX_STDOUT, IFX_LOG_ERROR, __VA_ARGS__)
#else
#define IFX_LOG_ERROR(...)
#endif

void ifx_log(FILE* f, ifx_Log_Severity_t s, char* msg, ...);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* IFX_RADAR_LOG_H */
