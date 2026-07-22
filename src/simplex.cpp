/*
 * Based on Java implementation code by Sébastien Rombauts from https://github.com/SRombauts/SimplexNoise
 *
 * "Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method by Stefan Gustavson in 2012.
 *
 * This could be speeded up even further, but it's useful as it is.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated."
 */


#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <random>

constexpr float G2 = 0.21132486540518713f;  // (3 - sqrt(3)) / 6
constexpr float F2 = 0.3660254037844386f;   // 0.5 * (sqrt(3) - 1)

constexpr float F3 = 1.0f / 3.0f;
constexpr float G3 = 1.0f / 6.0f;

constexpr std::array<std::array<int, 3>, 12> kGrad3 = {{
    {{1, 1, 0}}, {{-1, 1, 0}}, {{1, -1, 0}}, {{-1, -1, 0}},
    {{1, 0, 1}}, {{-1, 0, 1}}, {{1, 0, -1}}, {{-1, 0, -1}},
    {{0, 1, 1}}, {{0, -1, 1}}, {{0, 1, -1}}, {{0, -1, -1}},
}};

std::array<std::uint8_t, 512> gPerm = {};

bool gInitialized = false;

static int FastFloor(const float value);
static float Dot2(const std::array<int, 3>& gradient, const float x, const float y);
static void BuildPermutation(const std::uint32_t seed);
static float Noise2D(const float xin, const float yin, const std::uint32_t seed);
static float Fractal2D(const float x, const float y, const int octaves, const float lacunarity, const float gain, const std::uint32_t seed);

static int FastFloor(const float value)
{
    const int integer = static_cast<int>(value);
    return (value < static_cast<float>(integer)) ? (integer - 1) : integer;
}

static float Dot2(const std::array<int, 3>& gradient, const float x, const float y)
{
    return static_cast<float>(gradient[0]) * x + static_cast<float>(gradient[1]) * y;
}

static void BuildPermutation(const std::uint32_t seed)
{
    std::array<std::uint8_t, 256> source = {};
    std::iota(source.begin(), source.end(), static_cast<std::uint8_t>(0));

    std::mt19937 rng(seed);
    std::shuffle(source.begin(), source.end(), rng);

    for (std::size_t i = 0; i < 512; ++i)
    {
        gPerm[i] = source[i & 255U];
    }

    gInitialized = true;
}

static float Noise2D(const float xin, const float yin, const std::uint32_t seed)
{
    if (!gInitialized)
    {
        BuildPermutation(seed);
    }

    float n0 = 0.0f;
    float n1 = 0.0f;
    float n2 = 0.0f;

    const float skew = (xin + yin) * F2;
    const int i = FastFloor(xin + skew);
    const int j = FastFloor(yin + skew);

    const float unskew = static_cast<float>(i + j) * G2;
    const float x0 = xin - (static_cast<float>(i) - unskew);
    const float y0 = yin - (static_cast<float>(j) - unskew);

    int i1 = 0;
    int j1 = 0;
    if (x0 > y0)
    {
        i1 = 1;
        j1 = 0;
    }
    else
    {
        i1 = 0;
        j1 = 1;
    }

    const float x1 = x0 - static_cast<float>(i1) + G2;
    const float y1 = y0 - static_cast<float>(j1) + G2;
    const float x2 = x0 - 1.0f + 2.0f * G2;
    const float y2 = y0 - 1.0f + 2.0f * G2;

    const int ii = i & 255;
    const int jj = j & 255;
    const int gi0 = gPerm[ii + gPerm[jj]] % 12;
    const int gi1 = gPerm[ii + i1 + gPerm[jj + j1]] % 12;
    const int gi2 = gPerm[ii + 1 + gPerm[jj + 1]] % 12;

    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 >= 0.0f)
    {
        t0 *= t0;
        n0 = t0 * t0 * Dot2(kGrad3[gi0], x0, y0);
    }

    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 >= 0.0f)
    {
        t1 *= t1;
        n1 = t1 * t1 * Dot2(kGrad3[gi1], x1, y1);
    }

    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 >= 0.0f)
    {
        t2 *= t2;
        n2 = t2 * t2 * Dot2(kGrad3[gi2], x2, y2);
    }

    return 70.0f * (n0 + n1 + n2);
}

static float Fractal2D(
    const float x,
    const float y,
    const int octaves,
    const float lacunarity,
    const float gain,
    const std::uint32_t seed)
{
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float sum = 0.0f;
    float amplitude_sum = 0.0f;

    for (int octave = 0; octave < octaves; ++octave)
    {
        sum += Noise2D(x * frequency, y * frequency, seed) * amplitude;
        amplitude_sum += amplitude;
        amplitude *= gain;
        frequency *= lacunarity;
    }

    if (amplitude_sum == 0.0f)
    {
        return 0.0f;
    }

    return sum / amplitude_sum;
}

namespace Simplex
{
    float GenerateValue(int x, std::uint32_t seed, int octaves, float scale, float lacunarity, float gain)
    {
        return Fractal2D(x * scale, seed, octaves, lacunarity, gain, seed);
    }
}
