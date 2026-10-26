
#include <cassert>
#include <iostream>
#include <vector>
#include <array>

#include "dtmf/detector.hpp"
#include "tone_generator.hpp"

int main() {
    const double sample_rate = 8000.0;
    const double duration = 0.04;

    // TEST 1: Generate the DTMF signal for digit 5
    auto samples = dtmf::test::generate_dual_tone(
        770.0,
        1336.0,
        sample_rate,
        duration
    );

    // Ask our detector to identify the digit
    auto result = dtmf::detect_digit(
        samples,
        sample_rate
    );

    assert(result.has_value());
    assert(result.value() == '5');

    std::cout << "Digit 5 detected: "
              << result.value() << '\n';

    // TEST 2: Silence should not produce a digit
    std::vector<double> silence(320, 0.0);

    auto silence_result = dtmf::detect_digit(
        silence,
        sample_rate
    );

    assert(!silence_result.has_value());

    std::cout << "Silence correctly rejected\n";
    std::cout << "All detector tests PASSED!\n";

    
    // TEST 3: Verify all 12 DTMF keypad digits
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

    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 3; ++col) {

            auto tone = dtmf::test::generate_dual_tone(
                low_freqs[row],
                high_freqs[col],
                sample_rate,
                duration
            );

            auto detected = dtmf::detect_digit(
                tone,
                sample_rate
            );

            assert(detected.has_value());
            assert(detected.value() == keypad[row][col]);

            std::cout << "Expected: " << keypad[row][col]
                      << " | Detected: " << detected.value()
                      << '\n';
        }
    }

    std::cout << "All 12 keypad tests PASSED!\n";


    return 0;
}