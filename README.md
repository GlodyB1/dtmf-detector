# DTMF Detector — C++17

A C++ implementation of a Dual-Tone Multi-Frequency (DTMF) detector using the **Goertzel algorithm** for frequency analysis.

The project identifies telephone keypad digits by analyzing audio samples for the frequency pairs associated with each key. It includes synthetic signal generation, automated tests, and continuous integration through GitHub Actions.

## Features

- Detects all 12 standard telephone keypad keys (`0–9`, `*`, `#`).
- Uses the Goertzel algorithm to analyze individual DTMF frequencies.
- Rejects silence, extremely weak signals, and tested single-frequency inputs.
- Uses minimum-power and frequency-dominance checks to reduce invalid detections.
- Includes reproducible tests with generated sine waves and Gaussian noise.
- Supports automated builds and tests using CMake, CTest, and GitHub Actions.

## How DTMF Works

DTMF signaling represents each telephone keypad key using two simultaneous frequencies: one from a low-frequency group and another from a high-frequency group.

| | 1209 Hz | 1336 Hz | 1477 Hz |
|---|---|---|---|
| **697 Hz** | 1 | 2 | 3 |
| **770 Hz** | 4 | 5 | 6 |
| **852 Hz** | 7 | 8 | 9 |
| **941 Hz** | * | 0 | # |

For example, pressing **5** produces a combination of **770 Hz and 1336 Hz**.

The detector calculates the Goertzel power at each of the seven DTMF frequencies, identifies the strongest frequency in each group, applies signal validation checks, and maps the resulting pair to a keypad character.

## Technologies

- **Language:** C++17
- **Build system:** CMake
- **Testing:** CTest and C++ assertions
- **Continuous integration:** GitHub Actions
- **Development environment:** Ubuntu Linux / WSL 2
- **Signal processing:** Goertzel algorithm

## Building the Project

Requirements:

- A C++17-compatible compiler
- CMake

Clone the repository:

```bash
git clone https://github.com/GlodyB1/dtmf-detector.git
cd dtmf-detector
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

Run the automated tests:

```bash
ctest --test-dir build --output-on-failure
```

Run the detector tests directly to see individual results:

```bash
./build/test_detector
```

## Testing and Validation

The detector has been tested using synthetic DTMF signals sampled at **8,000 Hz**, with a **320-sample (40 ms)** analysis window.

Validation includes:

- Recognition of all 12 keypad digits
- Silence rejection
- Single-frequency rejection (770 Hz)
- Extremely weak signal rejection
- Detection of digit 5 with Gaussian noise

### Noise Tolerance Experiment

The detector was evaluated using synthetic signals for all 12 standard DTMF keys at an 8,000 Hz sample rate with 320-sample (40 ms) analysis windows.

For each noise level, 100 independently generated Gaussian noise sequences were applied to each key, producing 1,200 trials per noise level and **10,800 trials in total**. A fixed random seed was used for reproducibility.

| Noise standard deviation | Correct | Rejected | Incorrect | Accuracy |
|---|---:|---:|---:|---:|
| 0.0 | 1,200 | 0 | 0 | 100% |
| 0.1 | 1,200 | 0 | 0 | 100% |
| 0.2 | 1,200 | 0 | 0 | 100% |
| 0.4 | 1,200 | 0 | 0 | 100% |
| 0.6 | 1,200 | 0 | 0 | 100% |
| 0.8 | 1,171 | 29 | 0 | 97.58% |
| 1.0 | 1,009 | 191 | 0 | 84.08% |
| 1.5 | 458 | 742 | 0 | 38.17% |
| 2.0 | 189 | 1,007 | 4 | 15.75% |

**Key findings:**

- Achieved 100% detection accuracy across all 12 keys at tested noise standard deviations up to 0.6.
- Maintained 97.58% accuracy at noise standard deviation 0.8.
- Recorded only 4 incorrect digit classifications across 10,800 trials, all at the highest noise level.
- At high noise levels, the detector predominantly rejected signals rather than returning incorrect digits.

**Limitations:** These results apply to synthetically generated DTMF signals with additive Gaussian noise. Noise standard deviations are relative to the generated signal amplitude, not calibrated signal-to-noise ratios. Performance on real telephone recordings, different audio durations, and other interference types has not been established.

## Project Structure

```text
dtmf-detector/
├── include/dtmf/
│   ├── goertzel.hpp
│   └── detector.hpp
├── src/
│   ├── goertzel.cpp
│   └── detector.cpp
├── tests/
│   ├── tone_generator.hpp
│   ├── test_goertzel.cpp
│   └── test_detector.cpp
├── .github/workflows/
│   └── ci.yml
├── CMakeLists.txt
└── README.md
```

## Current Limitations

The current implementation processes audio sample vectors rather than reading audio files directly. Tests use generated signals, and the detector evaluates one analysis window at a time.

Potential future improvements include WAV file input, command-line usage, continuous digit-sequence recognition, and additional testing with real recordings.

## Author

Glody Bakareke

Computer Engineering Graduate — Kennesaw State University

[GitHub](https://github.com/GlodyB1)