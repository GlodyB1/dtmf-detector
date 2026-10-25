#pragma once

#include <vector>

namespace dtmf {

    // Calculate the power at a target frequency
    // samples: audio signal values
    //sample_rate: samples collected per second (Hz)
    // target_frequency: frequency to analyze (Hz)
    double goertzel_power(
        const std::vector<double>& samples,
        double sample_rate,
        double target_frequency
    );
}//namespace dtmf