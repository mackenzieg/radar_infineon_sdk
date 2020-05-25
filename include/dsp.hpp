#ifndef DSP_HPP
#define DSP_HPP

#include "ifxRadar_RangeSpectrum.h"
#include "ifxRadar_MTI.h"
#include "ifxRadar_Vector.h"
#include "ifxRadar_Matrix.h"
#include "radar_config.hpp"

class dsp
{
    public:
        dsp(radar_config *radar_config);
        virtual ~dsp();

    protected:

    private:
        typedef struct
        {
            ifx_Range_Spectrum_Handle_t range_spectrum_handle;

            ifx_Vector_R_t* fft_spectrum_result;

            ifx_Matrix_C_t* frame_fft_half_result;
        } range_spectrum_t;

        typedef struct
        {
            ifx_MTI_Handle_t mti_handle;

            ifx_Vector_R_t* mti_result;
        } mti_t;

        range_spectrum_t m_range_spectrum;

        mti_t m_mti;

        radar_config* m_radar_config;

        void create_spectrum_handle();

        void create_mti_handle();

        void run(ifx_Matrix_R_t *frame_data);
};

#endif // DSP_HPP
