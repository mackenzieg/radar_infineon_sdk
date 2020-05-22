#ifndef RANGE_SPECTRUM_HPP
#define RANGE_SPECTRUM_HPP

#include "ifxRadar_RangeSpectrum.h"
#include "radar_config.hpp"

class range_spectrum
{
    public:
        range_spectrum(radar_config *radar_config);
        virtual ~range_spectrum();

        void run(ifx_Matrix_R_t *frame_data);

    protected:
    private:

        radar_config* m_radar_config;
        // SDK Device handle
        ifx_Range_Spectrum_Handle_t m_range_spectrum_handle;

        ifx_Vector_R_t* m_fft_spectrum_result;

        ifx_Vector_R_t* m_frame_fft_half_result;
};

#endif //RANGE_SPECTRUM_HPP
