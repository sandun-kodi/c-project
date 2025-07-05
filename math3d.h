#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>

typedef struct {
    union {
        struct { float x, y, z; };
        struct { float r, theta, phi; };
    };
    int is_cartesian; // 1 for Cartesian, 0 for spherical
} vec3_t;

typedef struct {
    float m[16]; // Column-major 4x4 matrix
} mat4_t;

// Vector functions
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);

// Matrix functions
mat4_t mat4_identity();
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_xyz(float rx, float ry, float rz);
mat4_t mat4_frustum_asymmetric(float l, float r, float b, float t, float n, float f);


#endif

