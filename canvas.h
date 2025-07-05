#ifndef CANVAS_H
#define CANVAS_H

// Canvas structure to hold pixel brightness values (0.0 to 1.0)
typedef struct {
    int width;
    int height;
    float **pixels;
} canvas_t;

// Create a new canvas with given width and height
canvas_t *create_canvas(int width, int height);

// Free the memory used by the canvas
void free_canvas(canvas_t *canvas);

// Set a pixel with floating-point coordinates and intensity (bilinear filtering)
void set_pixel_f(canvas_t *canvas, float x, float y, float intensity);

// Draw a smooth line with floating-point endpoints and thickness
void draw_line_f(canvas_t *canvas, float x0, float y0, float x1, float y1, float thickness);

// Print the canvas as ASCII art (for debugging)
void print_canvas(canvas_t *canvas);

// Save the canvas as a PGM image file
void save_canvas_as_pgm(canvas_t *canvas, const char *filename);

#endif // CANVAS_H
