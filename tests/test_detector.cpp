
#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <algorithm>

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

    
    // TEST 4: A single frequency is not a valid DTMF digit
    auto single_tone = dtmf::test::generate_tone(
        770.0,
        sample_rate,
        duration
    );

    auto single_result = dtmf::detect_digit(
        single_tone,
        sample_rate
    );

    if (single_result.has_value()) {
        std::cout << "Single-tone test FAILED: detected "
                  << single_result.value()
                  << " instead of rejecting the signal\n";
    } else {
        std::cout << "Single-tone test PASSED: signal rejected\n";
    }

    assert(!single_result.has_value());

    
    // TEST 5: Reject an extremely weak DTMF signal
    auto weak_tone = dtmf::test::generate_dual_tone(
        770.0,
        1336.0,
        sample_rate,
        duration
    );

    // Reduce the signal amplitude to 1% of its original
    for (double& sample : weak_tone) {
        sample *= 0.01;
    }

    auto weak_result = dtmf::detect_digit(
        weak_tone,
        sample_rate
    );

    if (weak_result.has_value()) {
        std::cout << "Weak-signal test FAILED: detected "
                  << weak_result.value() << '\n';
    } else {
        std::cout << "Weak-signal test PASSED: signal rejected\n";
    }

    assert(!weak_result.has_value());

    
    // TEST 6: Detect digit 5 with added random noise
    auto noisy_tone = dtmf::test::generate_dual_tone(
        770.0,
        1336.0,
        sample_rate,
        duration
    );

    // Fixed seed makes the test reproducible
    std::mt19937 generator(42);

    // Gaussian noise with mean 0 and standard deviation 0.2
    std::normal_distribution<double> noise(0.0, 0.2);

    for (double& sample : noisy_tone) {
        sample += noise(generator);
    }

    auto noisy_result = dtmf::detect_digit(
        noisy_tone,
        sample_rate
    );

    if (noisy_result.has_value()) {
        std::cout << "Noisy-signal test: detected "
                  << noisy_result.value() << '\n';
    } else {
        std::cout << "Noisy-signal test: no digit detected\n";
    }

    assert(noisy_result.has_value());
    assert(noisy_result.value() == '5');

    std::cout << "Noisy-signal test PASSED!\n";

 
    
    // TEST 7: Measure accuracy across multiple noise trials
    const double noise_levels[] = {
        0.0, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 1.5, 2.0
    };

    constexpr int trials = 100;

    std::cout << "\nNoise accuracy experiment:\n";

    for (double noise_level : noise_levels) {
        int correct = 0;
        int rejected = 0;
        int incorrect = 0;

        std::mt19937 rng(42);

        for (int trial = 0; trial < trials; ++trial) {
            auto signal = dtmf::test::generate_dual_tone(
                770.0,
                1336.0,
                sample_rate,
                duration
            );

            if (noise_level > 0.0) {
                std::normal_distribution<double> noise(
                    0.0,
                    noise_level
                );

                for (double& sample : signal) {
                    sample += noise(rng);
                }
            }

            auto result = dtmf::detect_digit(
                signal,
                sample_rate
            );

            if (!result.has_value()) {
                ++rejected;
            } else if (result.value() == '5') {
                ++correct;
            } else {
                ++incorrect;
            }
        }

        double accuracy =
            100.0 * correct / trials;

        std::cout << "Noise: " << noise_level
                  << " | Accuracy: " << accuracy << "%"
                  << " | Correct: " << correct
                  << " | Rejected: " << rejected
                  << " | Incorrect: " << incorrect
                  << '\n';
    }

    return 0;
}