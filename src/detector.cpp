
#include "dtmf/detector.hpp"
#include "dtmf/goertzel.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

namespace dtmf {

std::optional<char> detect_digit(
    const std::vector<double>& samples,
    double sample_rate
) {
    constexpr std::array<double, 4> low_freqs = {
        697.0, 770.0, 852.0, 941.0
    };

    constexpr std::array<double, 3> high_freqs = {
        1209.0, 1336.0, 1477.0
    };

    constexpr char keypad[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    // Basic input validation
    if (samples.empty() ||
        !std::isfinite(sample_rate) ||
        sample_rate <= 2.0 * 1477.0) {
        return std::nullopt;
    }

    // Measure power at all seven DTMF frequencies
    std::array<double, 4> low_powers{};
    std::array<double, 3> high_powers{};

    for (std::size_t i = 0; i < low_freqs.size(); ++i) {
        low_powers[i] = goertzel_power(
            samples, sample_rate, low_freqs[i]
        );
    }

    for (std::size_t i = 0; i < high_freqs.size(); ++i) {
        high_powers[i] = goertzel_power(
            samples, sample_rate, high_freqs[i]
        );
    }

    // Find the strongest frequency in each group
    const auto low_it = std::max_element(
        low_powers.begin(), low_powers.end()
    );

    const auto high_it = std::max_element(
        high_powers.begin(), high_powers.end()
    );

    const std::size_t low_index =
        std::distance(low_powers.begin(), low_it);

    const std::size_t high_index =
        std::distance(high_powers.begin(), high_it);

    const double low_max = *low_it;
    const double high_max = *high_it;

    // Normalize power by the square of sample count
    const double n = static_cast<double>(samples.size());
    const double low_normalized = low_max / (n * n);
    const double high_normalized = high_max / (n * n);

    // Reject silence and very weak signals
    constexpr double min_power = 0.001;

    if (low_normalized < min_power ||
        high_normalized < min_power) {
        return std::nullopt;
    }

    // Check that the winning tones dominate their groups
    constexpr double dominance_ratio = 5.0;

    for (std::size_t i = 0; i < low_powers.size(); ++i) {
        if (i != low_index &&
            low_max < low_powers[i] * dominance_ratio) {
            return std::nullopt;
        }
    }

    for (std::size_t i = 0; i < high_powers.size(); ++i) {
        if (i != high_index &&
            high_max < high_powers[i] * dominance_ratio) {
            return std::nullopt;
        }
    }

    return keypad[low_index][high_index];
}

} // namespace dtmf
