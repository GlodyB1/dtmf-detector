#include "dtmf/goertzel.hpp"

#include <cmath>
#include <stdexcept>

namespace dtmf {

    double goertzel_power(
        const std::vector<double>& samples,
        double sample_rate,
        double target_frequency
    ) {
        // Validate the inputs
        if (samples.empty()) {
            throw std::invalid_argument("Samples cannot be empty.");
        }

        if (!std::isfinite(sample_rate) || sample_rate <= 0.0){
            throw std::invalid_argument("Invalid sample rate");
        }

        if (!std::isfinite(target_frequency) || target_frequency <= 0.0 || target_frequency >= sample_rate / 2.0){
            throw std::invalid_argument("Invalid target frequency");
        }

        // Calculate the normalized angular frequency
        constexpr double PI = 3.14159265358979323846;
        const double omega = 2.0 * PI * target_frequency / sample_rate;
        const double coefficient = 2.0 * std::cos(omega);

        // Previous two states
        double s1 = 0.0;
        double s2 = 0.0;

        // Process every audio sample
        for (double sample : samples){
            const double s = sample + coefficient * s1 - s2;

            s2 = s1;
            s1 = s;
        }

        // Calculate the power at the target frequency
        const double power = s1 * s1 +
                             s2 * s2 -
                             coefficient * s1 * s2;
                            
        return power;
    }
}//namespace dtmf