
#include <cassert>
#include <iostream>


#include "dtmf/goertzel.hpp"
#include "tone_generator.hpp"

int main() {
    // Test signal parameters
    const double sample_rate = 8000.0;
    const double duration = 0.04;
    const double frequency = 770.0;

    // Generate a 770 Hz sine wave
    auto samples = dtmf::test::generate_tone(
        frequency,
        sample_rate,
        duration
    );

    // Measure power at two different frequencies
    double power_770 = dtmf::goertzel_power(
        samples, sample_rate, 770.0
    );

    double power_941 = dtmf::goertzel_power(
        samples, sample_rate, 941.0
    );

    // Display results
    std::cout << "Power at 770 Hz: "
              << power_770 << '\n';

    std::cout << "Power at 941 Hz: "
              << power_941 << '\n';

    // Verify that the correct frequency dominates
    assert(power_770 > power_941 * 10.0);

    std::cout << "Goertzel test PASSED!\n";

    return 0;
}
