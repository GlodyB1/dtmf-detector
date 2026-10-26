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
    inline std::vector<double> generate_dual_tone(
        double low_frequency,
        double high_frequency,
        double sample_rate,
        double duration_seconds
    ) {
        // Generate each tone separately
        auto low = generate_tone(
            low_frequency,
            sample_rate,
            duration_seconds,
            0.5 // Amplitude for low frequency
        );

        auto high = generate_tone(
            high_frequency,
            sample_rate,
            duration_seconds,
            0.5 // Amplitude for high frequency
        );

        // Combine the two signals
        for (std::size_t i = 0; i < low.size(); ++i) {
            low[i] += high[i];
        }
        return low;
    }

}//namespace dtmf::test