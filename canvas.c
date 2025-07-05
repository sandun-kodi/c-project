#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

// Canvas structure as per assignment
canvas_t *create_canvas(int width, int height) {
    canvas_t *canvas = malloc(sizeof(canvas_t));
    if (!canvas) return NULL;

    canvas->width = width;
    canvas->height = height;
    canvas->pixels = malloc(height * sizeof(float *));
    if (!canvas->pixels) {
        free(canvas);
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        canvas->pixels[i] = calloc(width, sizeof(float));
        if (!canvas->pixels[i]) {
            for (int j = 0; j < i; j++) free(canvas->pixels[j]);
            free(canvas->pixels);
            free(canvas);
            return NULL;
        }
    }
    return canvas;
}

// Bilinear filter set pixel (subpixel, smooth)
void set_pixel_f(canvas_t *canvas, float x, float y, float intensity) {
    int x0 = (int)floorf(x), y0 = (int)floorf(y);
    int x1 = x0 + 1, y1 = y0 + 1;
    float dx = x - x0, dy = y - y0;

    // Distribute intensity to 4 nearest pixels
    if (x0 >= 0 && x0 < canvas->width && y0 >= 0 && y0 < canvas->height) {
        canvas->pixels[y0][x0] += intensity * (1 - dx) * (1 - dy);
        if (canvas->pixels[y0][x0] > 1.0f) canvas->pixels[y0][x0] = 1.0f;
    }
    if (x1 >= 0 && x1 < canvas->width && y0 >= 0 && y0 < canvas->height) {
        canvas->pixels[y0][x1] += intensity * dx * (1 - dy);
        if (canvas->pixels[y0][x1] > 1.0f) canvas->pixels[y0][x1] = 1.0f;
    }
    if (x0 >= 0 && x0 < canvas->width && y1 >= 0 && y1 < canvas->height) {
        canvas->pixels[y1][x0] += intensity * (1 - dx) * dy;
        if (canvas->pixels[y1][x0] > 1.0f) canvas->pixels[y1][x0] = 1.0f;
    }
    if (x1 >= 0 && x1 < canvas->width && y1 >= 0 && y1 < canvas->height) {
        canvas->pixels[y1][x1] += intensity * dx * dy;
        if (canvas->pixels[y1][x1] > 1.0f) canvas->pixels[y1][x1] = 1.0f;
    }
}

// DDA line drawing with thickness
void draw_line_f(canvas_t *canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0, dy = y1 - y0;
    float length = sqrtf(dx * dx + dy * dy);
    float steps = ceilf(length) * 5.0f;

    float x_inc = dx / steps, y_inc = dy / steps;
    float perp_x = -dy / length, perp_y = dx / length;

    float x = x0, y = y0;
    for (int i = 0; i <= steps; i++) {
        set_pixel_f(canvas, x, y, 1.0f);
        for (int t = 1; t < (int)thickness; t++) {
            set_pixel_f(canvas, x + t * perp_x, y + t * perp_y, 1.0f);
            set_pixel_f(canvas, x - t * perp_x, y - t * perp_y, 1.0f);
        }
        x += x_inc;
        y += y_inc;
    }
}

// Utility: Print as ASCII
void print_canvas(canvas_t *canvas) {
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            float intensity = canvas->pixels[y][x];
            char pixel = intensity > 0.8 ? '#' : (intensity > 0.5 ? '*' : (intensity > 0.2 ? '.' : ' '));
            printf("%c", pixel);
        }
        printf("\n");
    }
}

// Utility: Save as PGM
void save_canvas_as_pgm(canvas_t *canvas, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "P2\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            fprintf(file, "%d ", (int)(canvas->pixels[y][x] * 255));
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
