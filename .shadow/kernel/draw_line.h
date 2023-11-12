#ifndef DRAW_LINE_H
#define DRAW_LINE_H

#include <stdint.h>
// #include <GL/glut.h>

// Function to draw a pixel
void draw_pixel(int x, int y);

// Function to draw a line using Bresenham's algorithm
void draw_line_bresenham(int x1, int y1, int x2, int y2, uint32_t color, int bold);

// Function to draw a line using DDA algorithm
void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color, int bold);

// Function to draw a line using Midpoint algorithm
void draw_line_midpoint(int x1, int y1, int x2, int y2, uint32_t color, int bold);

void draw_line(int x1, int y1, int x2, int y2, uint32_t color, int bold, int algorithm);

#endif // DRAW_LINE_H
