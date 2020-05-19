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
 * @file ifxRadar_PeakSearch.h
 *
 * @brief This file defines the API for peak-search module. In general peak search
 *        function is going to search the peaks from the given data with given
 *        conditions.
 *
 */


#ifndef IFX_RADAR_PEAK_SEARCH_H
#define IFX_RADAR_PEAK_SEARCH_H

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
 * @image html peak_search_module.png "Example showing config params for ifx_peak_search_create (config params in bold)" width=800cm
 *
 */
typedef struct ifx_Peak_Search_Config_s
{
    ifx_Float_t value_per_bin;      /**<This is the value quantity 'x' represented by each bin (or each sample) of the input data set.
                                                For e.g., if the input data set is a range spectrum, then this value gives the
                                                range represented by each sample of the data set.*/
    ifx_Float_t search_zone_start;  /**<This specifies the start value of a section of the input data where the peak
                                                search needs to be made.
                                                e.g., if the data set is a range spectrum this value specifies the starting range
                                                value of the search zone.*/
    ifx_Float_t search_zone_end;    /**<This specifies the end value of a section of the input data where the peak
                                                search needs to be made.
                                                e.g., if the data set is a range spectrum this value specifies the end range
                                                value of the search zone.*/
    ifx_Float_t threshold_factor;   /**<The peak search incorporates an adaptive threshold value of 'y' below which any peaks are
                                                ignored. This factor is multiplied with the mean value of the entire input data set to get
                                                an adaptive threshold value.<*/
    ifx_Float_t threshold_offset;   /**<This value is added to the value obtained by multiplying the threshold_factor with
                                                the mean of the data set to get the final 'y' threshold.*/
    uint32_t    max_num_peaks;      /**< This decides the max number of peaks to be identified in the search zone.*/
} ifx_Peak_Search_Config_t;

typedef struct
{
    uint32_t  peak_count; /**< Number of found peaks. */
    uint32_t* index;      /**< Array of indices of found peaks*/
} ifx_Peak_Search_Result_t;
/**
 *
 * @brief Forward declaration structure for peak-search module.
 *
 */
typedef struct ifx_Peak_Search_s* ifx_Peak_Search_Handle_t;

/*
==============================================================================
   5. FUNCTION PROTOTYPES
==============================================================================
*/

/**
 * @brief This function creates a peak search handle with the some input parameters that decide the nature of the
 * peak search.
 *
 * @image html peak_search_module.png "Example showing params from ifx_Peak_Search_Config_t (params in bold)" width=800cm
 *
 * @param [in]     config    The peak search settings containing the rules
 *                            on how peak search is applied.
 * @param [in,out] handle     The address of a valid peak search handle.
 * @return ifx_Error_t        Error Code
 */
ifx_Error_t ifx_peak_search_create(const ifx_Peak_Search_Config_t* config,
                                   ifx_Peak_Search_Handle_t* handle);

/**
 * @brief Destroy the peak search handle.
 *
 * @param [in,out] handle Valid peak search handle pointer.
 * @return ifx_Error_t    Error Code
 */
ifx_Error_t ifx_peak_search_destroy(ifx_Peak_Search_Handle_t handle);

/**
 * @brief Using a valid peak search handle to search peaks from input data_set.
 * The run function looks for peaks in the data_set vector between \ref ifx_Peak_Search_Config_s.search_zone_start
 * and \ref ifx_Peak_Search_Config_s.search_zone_end, as long as the peak values are higher than a threshold
 * obtained by multiplying the mean value of the data_set with \ref ifx_Peak_Search_Config_s.threshold_factor and
 * adding \ref ifx_Peak_Search_Config_s.threshold_offset to it. The peaks are computed by comparing with
 * 2 neighbouring values on either side of every sample value within the search zone.
 * The peak search stops once the entire search zone has been parsed for peaks OR once
 * \ref ifx_Peak_Search_Config_s.max_num_peaks are encountered, whichever is earlier.
 *
 * @param [in,out]  handle    Valid peak search handle.
 * @param [in]      data_set  The target data set to search for peaks.
 * @param [out]     result    Result of the peak search.
 * @return ifx_Error_t        Error Code
 */
ifx_Error_t ifx_peak_search_run(ifx_Peak_Search_Handle_t handle,
                                const ifx_Vector_R_t* data_set,
                                ifx_Peak_Search_Result_t* result);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif  /* IFX_RADAR_PEAK_SEARCH_H */


