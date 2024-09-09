#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <optional>

struct PpmHeader {
    char magic[3];
    int width;
    int height;
    uint16_t max_value;
};

struct PpmPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    auto operator<=>(const PpmPixel&) const = default;
};

std::optional<std::tuple<PpmHeader, std::vector<PpmPixel>>> parse_ppm(const std::string&);
