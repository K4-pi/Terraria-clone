#pragma once

#include <cstdint>

namespace Simplex
{
    float GenerateValue(int x, std::uint32_t seed, int octaves, float scale, float lacunarity, float gain);
}
