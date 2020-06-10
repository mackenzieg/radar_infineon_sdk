#ifndef FFT_CIRCULAR_HPP
#define FFT_CIRCULAR_HPP

#define NUM_FFT_POINTS 2048

#include <fftw3.h>

class fft_circular {
    public:
        fft_circular();
        virtual ~fft_circular();

        void sample(ifx_Complex_t element);

        fftw_complex get_result();
    protected:
    private:
        fftw_complex signal[NUM_FFT_POINTS];
        fftw_complex result[NUM_FFT_POINTS];

        fftw_plan plan;

        uint32_t sample_count = 0;

        bool result_valid = false;
};


#endif //FFT_CIRCULAR_HPP
