#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "canvas.h"

int main() {
    int width = 800, height = 800;
    canvas_t *canvas = create_canvas(width, height);

    if (!canvas) {
        printf("Failed to create canvas!\n");
        return 1;
    }

    float cx = width / 2.0f;  // Center X
    float cy = height / 2.0f;  // Center Y
    float radius = width * 0.4f;
    float thickness = 3.0f;

    // Draw lines outward like a clock face
    for (int angle = 0; angle < 360; angle += 15) {
        float rad = angle * 3.14159 / 180.0f; // Convert degrees to radians
        float x1 = cx + radius * cos(rad);
        float y1 = cy + radius * sin(rad);
        draw_line_f(canvas, cx, cy, x1, y1, thickness);
    }

    // Save the image as PGM format
    save_canvas_as_pgm(canvas, "clock.pgm");
    printf("Clock face saved as 'clock.pgm'\n");

    return 0;
}

