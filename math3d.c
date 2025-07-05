#include "math3d.h"

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.x = r * sinf(phi) * cosf(theta);
    v.y = r * sinf(phi) * sinf(theta);
    v.z = r * cosf(phi);
    v.is_cartesian = 1;
    return v;
}

vec3_t vec3_normalize_fast(vec3_t v) {
    float inv_len = 1.0f / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); // or use fast inverse sqrt
    v.x *= inv_len;
    v.y *= inv_len;
    v.z *= inv_len;
    return v;
}

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    float dot = a.x*b.x + a.y*b.y + a.z*b.z;
    dot = fmaxf(fminf(dot, 1.0f), -1.0f); // clamp

    float theta = acosf(dot) * t;
    vec3_t rel = { b.x - a.x * dot, b.y - a.y * dot, b.z - a.z * dot };
    rel = vec3_normalize_fast(rel);

    vec3_t result = {
        a.x * cosf(theta) + rel.x * sinf(theta),
        a.y * cosf(theta) + rel.y * sinf(theta),
        a.z * cosf(theta) + rel.z * sinf(theta)
    };
    return result;
}

mat4_t mat4_identity() {
    mat4_t m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t m = mat4_identity();
    m.m[12] = tx;
    m.m[13] = ty;
    m.m[14] = tz;
    return m;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t m = {0};
    m.m[0] = sx; m.m[5] = sy; m.m[10] = sz; m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_rotate_xyz(float rx, float ry, float rz) {
    float cx = cosf(rx), sx = sinf(rx);
    float cy = cosf(ry), sy = sinf(ry);
    float cz = cosf(rz), sz = sinf(rz);

    mat4_t m = mat4_identity();
    m.m[0] = cy * cz;
    m.m[1] = sx * sy * cz - cx * sz;
    m.m[2] = cx * sy * cz + sx * sz;

    m.m[4] = cy * sz;
    m.m[5] = sx * sy * sz + cx * cz;
    m.m[6] = cx * sy * sz - sx * cz;

    m.m[8] = -sy;
    m.m[9] = sx * cy;
    m.m[10] = cx * cy;
    return m;
}

mat4_t mat4_frustum_asymmetric(float l, float r, float b, float t, float n, float f) {
    mat4_t m = {0};
    m.m[0] = 2.0f * n / (r - l);
    m.m[5] = 2.0f * n / (t - b);
    m.m[8] = (r + l) / (r - l);
    m.m[9] = (t + b) / (t - b);
    m.m[10] = -(f + n) / (f - n);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * f * n) / (f - n);
    return m;
}


