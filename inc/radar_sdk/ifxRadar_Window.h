/* ===========================================================================
** Copyright (C) 2018 Infineon Technologies AG. All rights reserved.
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
 * @file ifxRadar_Window.h
 *
 * @brief This file defines the API to configure the coefficients of
 *        various Window Functions
 *
 *
 */


#ifndef IFX_RADAR_WINDOW_H
#define IFX_RADAR_WINDOW_H

/*
==============================================================================
   1. INCLUDE FILES
==============================================================================
*/
#include "ifxRadar_Vector.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**
 *@internal
 * defgroup is missing, all this is API relevant, so it needs to appear
 * in the doxy module documentation. See also ifxRadar_sdk.h for an example.
 *@endinternal
 */
/**
 *@internal
 * no astyling, no linting yet ... as far as I can see. Not even -Wall on
 * gcc command line :-(
 *@endinternal
 */
/*
==============================================================================
   2. DEFINITIONS
==============================================================================
*/

/*
==============================================================================
   3. TYPES
==============================================================================
*/

/**
 *
 * @brief Defines supported Window options.
 *        Use type ifx_Window_Type_t for this enum.
 *
 */
typedef enum
{
    WINDOW_HAMM           = 0U,    /**< Hamming window */
    WINDOW_HANN           = 1U,    /**< Hanning window */
    WINDOW_BLACKMANHARRIS = 2U,    /**< Blackmann Harris window */
    WINDOW_CHEBYSHEV      = 3U,    /**< Chebyshev window */
} ifx_Window_Type_t;

/**
 * @brief Defines window related settings.
 *        Use type ifx_Window_Config_t for this enum.
 *
 */
typedef struct
{
    ifx_Window_Type_t type;  /**< Type of window function defined by \ref ifx_Window_Type_t */
    uint32_t          size;  /**< Number of elements in the window */
    ifx_Float_t       at_dB; /**< Attenuation parameter: in case of Chebyshev window */
} ifx_Window_Config_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief This function generates the coefficients of a user selected window
 *        for a given length.
 *
 * @param [in]      config  Stucture defining the type of window, the number of elements in the window and also
 *                          any additional parameter specific to certain window type.
 * @param [in, out] win     Pointer to an allocated and populated vector instance defined by \ref ifx_Vector_R_t
 *                          filled with Window coefficients defined in \ref ifx_Window_Config_t.
 * @return ifx_Error_t      Error code.
 */
ifx_Error_t ifx_window_init(const ifx_Window_Config_t* config,
                            ifx_Vector_R_t* win);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* IFX_RADAR_WINDOW_H */

