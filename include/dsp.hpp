#ifndef DSP_HPP
#define DSP_HPP

#include "ifxRadar_RangeSpectrum.h"
#include "ifxRadar_MTI.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Matrix.h"
#include "radar_config.hpp"
#include "ifxRadar_Frame.h"
#include "ifxRadar_PeakSearch.h"

class dsp
{
    public:
        dsp(radar_config *radar_config);
        virtual ~dsp();

        void run(ifx_Frame_t frame);

    protected:

    private:
        typedef struct
        {
            ifx_Range_Spectrum_Handle_t range_spectrum_handle;

            ifx_Vector_R_t fft_spectrum_result;

            ifx_Matrix_C_t frame_fft_half_result;
        } range_spectrum_t;

        typedef struct
        {
            ifx_MTI_Handle_t mti_handle;

            ifx_Vector_R_t mti_result;
        } mti_t;

        typedef struct
        {
            ifx_FFT_Handle_t doppler_fft_handle;

            ifx_Vector_C_t doppler_data;

            ifx_Vector_C_t chirp_fft_result;
        } doppler_fft_t;


        range_spectrum_t m_range_spectrum;

        mti_t m_mti;

        doppler_fft_t m_doppler_fft;

        radar_config* m_radar_config;

        int run_count = 0;

        void create_spectrum_handle();
        void destroy_spectrum_handle();

        void create_mti_handle();
        void destroy_mti_handle();

        void create_range_doppler_handle();
        void destroy_range_doppler_handle();

        void create_doppler_fft_handle();
        void destroy_doppler_fft_handle();

        float create_scale(ifx_Vector_R_t* win);
        void fft_shift(ifx_Vector_C_t* vector);
};

#endif // DSP_HPP
