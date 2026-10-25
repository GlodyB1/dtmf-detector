#pragma once

#include <cmath>
#include <cstddef>
#include <vector>

namespace dtmf::test{

    inline std::vector<double> generate_tone(
        double frequency,
        double sample_rate,
        double duration_seconds,
        double amplitude = 1.0
    ) {
        
        constexpr double PI = 3.14159265358979323846;

        const std::size_t sample_count = static_cast<std::size_t>(sample_rate * duration_seconds);

        std::vector<double> samples;
        samples.reserve(sample_count);

        for(std::size_t n = 0; n < sample_count; ++n) {
            const double time = static_cast<double>(n) / sample_rate;

            const double sample = amplitude * std::sin(2.0 * PI * frequency * time);

            samples.push_back(sample);
        }

        return samples;
    }

}//namespace dtmf::test