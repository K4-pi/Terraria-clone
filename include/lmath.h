#pragma once

typedef struct {
    float x, y;
} vector2f_t;

typedef struct {
    int x, y;
} vector2_t;

typedef struct {
    float x, y, w, h;
} texture_coordinates_t;  // Used for one place store of texture position in file
