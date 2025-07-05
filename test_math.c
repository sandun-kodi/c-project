#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"

#define M_PI 3.14159265358979323846
#define WIDTH 800
#define HEIGHT 800

vec3_t cube_vertices[8] = {
    {-1, -1, -1}, {1, -1, -1},
    {1,  1, -1}, {-1, 1, -1},
    {-1, -1,  1}, {1, -1,  1},
    {1,  1,  1}, {-1, 1,  1}
};

int cube_edges[12][2] = {
    {0,1}, {1,2}, {2,3}, {3,0},
    {4,5}, {5,6}, {6,7}, {7,4},
    {0,4}, {1,5}, {2,6}, {3,7}
};

// Project a 3D point to 2D (orthographic)
void project(vec3_t v, float *x, float *y) {
    *x = WIDTH / 2 + v.x * 100;
    *y = HEIGHT / 2 - v.y * 100;
}

// Matrix-vector multiplication (vec3)
vec3_t mat4_mul_vec3(mat4_t m, vec3_t v) {
    vec3_t r;
    r.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12];
    r.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13];
    r.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    return r;
}

// Matrix-matrix multiplication (column-major)
mat4_t mat4_multiply(mat4_t a, mat4_t b) {
    mat4_t result = {0};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[col + row * 4] += a.m[i + row * 4] * b.m[col + i * 4];
            }
        }
    }
    return result;
}

int main() {

    mkdir("frames", 0777);

    for (int i = 0; i < 20; i++){
        canvas_t *canvas = create_canvas(WIDTH, HEIGHT);
        if (!canvas) {
            fprintf(stderr, "Failed to create canvas.\n");
            return 1;
        }

        float angle = i * (2*M_PI / 20);

        // Transformation matrix: Scale -> Rotate -> Translate
        mat4_t scale = mat4_scale(1.5, 1.5, 1.5);
        mat4_t rotate = mat4_rotate_xyz(angle, angle, angle);
        mat4_t translate = mat4_translate(0, 0, 0);

        mat4_t transform = mat4_multiply(translate, mat4_multiply(rotate, scale)); 

        // Transform cube vertices
        vec3_t transformed[8];
        for (int i = 0; i < 8; i++) {
            transformed[i] = mat4_mul_vec3(transform, cube_vertices[i]);
        }

        // Draw edges
        for (int i = 0; i < 12; i++) {
            float x0, y0, x1, y1;
            project(transformed[cube_edges[i][0]], &x0, &y0);
            project(transformed[cube_edges[i][1]], &x1, &y1);
            draw_line_f(canvas, x0, y0, x1, y1, 3.0f);
        }

        char filename[64];
        snprintf(filename, sizeof(filename), "frames/cube_%02d.pgm", i);
        save_canvas_as_pgm(canvas, filename);
        printf("Cube saved as 'cube_%02d.pgm' in frames/\n", i);

        // Clean up
        for (int i = 0; i < canvas->height; i++) free(canvas->pixels[i]);
        free(canvas->pixels);
        free(canvas);
    }

    return 0;
}
