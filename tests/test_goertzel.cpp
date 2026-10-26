
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

    
    // Test a dual-tone DTMF signal (digit 5)
    auto dual_samples = dtmf::test::generate_dual_tone(
        770.0, 1336.0, sample_rate, duration
    );

    double dual_power_770 = dtmf::goertzel_power(
        dual_samples, sample_rate, 770.0
    );

    double dual_power_1336 = dtmf::goertzel_power(
        dual_samples, sample_rate, 1336.0
    );

    double dual_power_941 = dtmf::goertzel_power(
        dual_samples, sample_rate, 941.0
    );

    std::cout << "\nDual-tone test (digit 5):\n";
    std::cout << "770 Hz power: " << dual_power_770 << '\n';
    std::cout << "1336 Hz power: " << dual_power_1336 << '\n';
    std::cout << "941 Hz power: " << dual_power_941 << '\n';

    // Both expected frequencies should dominate
    assert(dual_power_770 > dual_power_941 * 10.0);
    assert(dual_power_1336 > dual_power_941 * 10.0);

    std::cout << "Dual-tone test PASSED!\n";

    return 0;
}
