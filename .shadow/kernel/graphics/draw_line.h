#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include <stdint.h>

// Function to draw a pixel
void draw_pixel(int x, int y, uint32_t color, int bold, int pixel_side);

// Function to draw a line using Bresenham's algorithm
void draw_line_bresenham(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side);

// Function to draw a line using DDA algorithm
void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side);

// Function to draw a line using Midpoint algorithm
void draw_line_midpoint(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side);

void draw_line(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side, int algorithm);

#endif // DRAW_LINE_H
