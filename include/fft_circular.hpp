#ifndef FFT_CIRCULAR_HPP
#define FFT_CIRCULAR_HPP

#define NUM_FFT_POINTS 512

#include <fftw3.h>

#include "ifxRadar_Vector.h"

class fft_circular {
    public:
        fft_circular();
        virtual ~fft_circular();

        void sample(ifx_Complex_t element);

        fftw_complex* get_signal();
        fftw_complex* get_result();
    protected:
    private:
        fftw_complex* signal;
        fftw_complex* result;

        fftw_plan plan;

        uint32_t sample_count = 0;

        bool result_valid = false;
};

#endif //FFT_CIRCULAR_HPP
