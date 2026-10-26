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

To explore robustness, 100 noisy versions of digit 5 were evaluated at each Gaussian noise standard deviation.

| Noise standard deviation | Correct detections | Accuracy |
|---|---:|---:|
| 0.0 | 100/100 | 100% |
| 0.1 | 100/100 | 100% |
| 0.2 | 100/100 | 100% |
| 0.4 | 100/100 | 100% |
| 0.6 | 99/100 | 99% |
| 0.8 | 95/100 | 95% |
| 1.0 | 82/100 | 82% |
| 1.5 | 37/100 | 37% |
| 2.0 | 16/100 | 16% |

No incorrect digits were returned in these 900 trials; unsuccessful trials returned no detection.

**Limitations:** These results reflect synthetic digit-5 signals and a fixed-seed Gaussian noise model. They do not establish accuracy across all digits, real telephone recordings, or arbitrary interference conditions. Noise levels are expressed as standard deviations relative to the generated signal amplitude, not calibrated SNR values.

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