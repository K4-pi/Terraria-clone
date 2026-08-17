#pragma once

struct vector2f_t {
    float x, y;
};

struct vector2_t {
    int x, y;

    bool operator == (const vector2_t &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct texture_coordinates_t {
    float x, y, w, h;
};  // Used for one place store of texture position in file
