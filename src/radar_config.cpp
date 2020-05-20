#include "radar_config.hpp"

radar_config::radar_config()
{
    m_device_metrics.m_range_resolution = 0.15f;
    m_device_metrics.m_maximum_range = 9.59f;
    m_device_metrics.m_speed_resolution = 0.08f;
    m_device_metrics.m_maximum_speed = 2.45f;

    m_device_metrics.m_frame_rate = 5;
    m_device_metrics.m_adc_samplerate_hz = 1000000;
    m_device_metrics.m_bgt_tx_power = 31;
    m_device_metrics.m_rx_antenna_number = 3;
    m_device_metrics.m_if_gain_db = 33;

    m_device_metrics.m_fmcw_center_frequency_khz = 60500000;

    compute_metrics();
}

radar_config::~radar_config()
{
    //dtor
}

void radar_config::compute_metrics()
{
    const double c0 = 2.99792458e8;
        /*
     * The range transform is an FFT. Here it is assumed that no zero padding is applied, because
     * zero is just interpolation and does not increase the true resolution.
     *
     * FMCW maps distance to a frequency in time domain data. During a chirp the RF travels from the
     * sensor to the target and back. With the distance r between sensor and target the signal travel
     * time is t = 2r / c0. During the time t the RF frequency has increased by f = t * BW/T, where
     * BW is the full bandwidth of the chirp and T is the chirp time. Putting this together distance
     * is mapped to frequency according to this formula: r = c0 * T * f / (2 * BW).
     *
     * With N samples acquired at f_sr, the frequency resolution per bin is df = f_sr / N, so the
     * range resolution results in dr = c0 * T * f_sr / (2 * N * BW).
     *
     * Ideally the chirp time is equal to the total ADC sampling duration T=N/f_sr and the resolution
     * formula collapses to dr = c0 / (2 * BW). This formula is used here. With a real sensor the
     * relationship T = N/f_sr may not be correct, because due to settling processes at the beginning
     * of the chirp the ADC is enabled with some delay after the ramp start.
     */
    double bandwidth_khz = 0.001 * c0 / (2 * m_device_metrics.m_range_resolution);

    m_device_config.lower_frequency_kHz = m_device_metrics.m_fmcw_center_frequency_khz - (uint32_t)(bandwidth_khz * 0.5 + 0.5);
    m_device_config.upper_frequency_kHz = m_device_metrics.m_fmcw_center_frequency_khz + (uint32_t)(bandwidth_khz * 0.5 + 0.5);

        /*
     * The number of bins multiplied with the range resolution results in the total range. Due to
     * Nyquist theorem only half of the spectrum is evaluated in range transform so the total range
     * is reduced by a factor of 2.
     *
     * Range transform is an FFT, and usually FFT sizes are powers of 2. If number of samples is
     * not a power of two, the FFT input could be zero padded. Anyway here the number of samples
     * is rounded to the next power of two increasing the total range. The trick to calculate this
     * rounding was found here: https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2.
     */
    m_device_config.num_samples_per_chirp = (uint32_t) (2.0f * m_device_metrics.m_maximum_range / m_device_metrics.m_range_resolution);

    m_device_config.num_samples_per_chirp--;
    m_device_config.num_samples_per_chirp |= m_device_config.num_samples_per_chirp >> 1;
    m_device_config.num_samples_per_chirp |= m_device_config.num_samples_per_chirp >> 2;
    m_device_config.num_samples_per_chirp |= m_device_config.num_samples_per_chirp >> 4;
    m_device_config.num_samples_per_chirp |= m_device_config.num_samples_per_chirp >> 8;
    m_device_config.num_samples_per_chirp |= m_device_config.num_samples_per_chirp >> 16;
    m_device_config.num_samples_per_chirp++;

    /*
     * This formula was provided by algorithm team. Detailed information about this may be found
     * in some papers. At this point there is no documentation because the implementor of this
     * function does not know those papers. Sorry.
     */
    const double lambda = c0 / (1000.f * m_device_metrics.m_fmcw_center_frequency_khz);
    m_device_config.chirp_to_chirp_time_100ps = (uint64_t) (1.0e10 * lambda / (double)(4.0f * m_device_metrics.m_maximum_speed));

    /*
     * The number of bins multiplied with the speed resolution results in the maximum speed. The
     * bins of the Doppler transforms represent the -v_max...v_max, that's why the maximum speed
     * is divided by 2.
     *
     * Doppler transform is an FFT, and usually FFT sizes are powers of 2. If number of samples is
     * not a power of two, the FFT input could be zero padded. Anyway here the number of samples
     * is rounded to the next power of two increasing the total range. The trick to calculate this
     * rounding was found here: https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2.
     */
    m_device_config.num_chirps_per_frame = (uint32_t) (2.0f * m_device_metrics.m_maximum_speed / m_device_metrics.m_speed_resolution);

    m_device_config.num_chirps_per_frame--;
    m_device_config.num_chirps_per_frame |= m_device_config.num_chirps_per_frame >> 1;
    m_device_config.num_chirps_per_frame |= m_device_config.num_chirps_per_frame >> 2;
    m_device_config.num_chirps_per_frame |= m_device_config.num_chirps_per_frame >> 4;
    m_device_config.num_chirps_per_frame |= m_device_config.num_chirps_per_frame >> 8;
    m_device_config.num_chirps_per_frame |= m_device_config.num_chirps_per_frame >> 16;
    m_device_config.num_chirps_per_frame++;

    m_device_config.frame_period_us   = (uint64_t) (1.0e6f / m_device_metrics.m_frame_rate);
    m_device_config.adc_samplerate_hz = m_device_metrics.m_adc_samplerate_hz;
    m_device_config.bgt_tx_power      = m_device_metrics.m_bgt_tx_power;
    m_device_config.rx_antenna_mask   = (0x01 << (m_device_metrics.m_rx_antenna_number - 1));
    m_device_config.if_gain_dB        = m_device_metrics.m_if_gain_db;
}

ifx_Device_Config_t* radar_config::get_device_config()
{
    return &m_device_config;
}
