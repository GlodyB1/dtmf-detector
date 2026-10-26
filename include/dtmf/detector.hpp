#pragma once

#include <optional>
#include <vector>

namespace dtmf {

    // Returns a digit if a valid DTMF tone is detected.
    // Returns std::nullopt if no valid digit is found.
    std::optional<char> detect_digit(
        const std::vector<double>& samples,
        double sample_rate
    );

} //namespace dtmf