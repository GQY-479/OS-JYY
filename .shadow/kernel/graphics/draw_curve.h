// #include <stdio.h>
// #include <math.h>
#include "draw_tile.h"

// Function to calculate the quadratic Bezier curve point
void quadraticBezier(float x0, float y0, float x1, float y1, float x2, float y2, float t, float* x, float* y) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float ut = u * t;

    *x = uu * x0 + 2 * ut * x1 + tt * x2;
    *y = uu * y0 + 2 * ut * y1 + tt * y2;
}

// Function to draw a quadratic Bezier curve
void drawQuadraticBezier(float x0, float y0, float x1, float y1, float x2, float y2, uint32_t color) {
    int numSegments = 100; // Number of line segments to approximate the curve

    for (int i = 0; i <= numSegments; i++) {
        float t = i / (float)numSegments;
        float x, y;
        quadraticBezier(x0, y0, x1, y1, x2, y2, t, &x, &y);
        int pixel_x = (int)x;
        int pixel_y = (int)y;
        draw_tile(pixel_x, pixel_y, 1, 1, color);
    }
}

// Function to calculate the cubic Bezier curve point
void cubicBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float t, float* x, float* y) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    *x = uuu * x0 + 3 * uu * t * x1 + 3 * u * tt * x2 + ttt * x3;
    *y = uuu * y0 + 3 * uu * t * y1 + 3 * u * tt * y2 + ttt * y3;
}

// Function to draw a cubic Bezier curve
void drawCubicBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {
    int numSegments = 100; // Number of line segments to approximate the curve

    for (int i = 0; i <= numSegments; i++) {
        float t = i / (float)numSegments;
        float x, y;
        cubicBezier(x0, y0, x1, y1, x2, y2, x3, y3, t, &x, &y);
        int pixel_x = (int)x;
        int pixel_y = (int)y;
        draw_tile(pixel_x, pixel_y, 1, 1, color);
    }
}


// Function to draw a smooth Bezier curve using segment-wise cubic Bezier curves
void drawSmoothBezier(float* pointsX, float* pointsY, int numPoints, uint32_t color) {
    int numSegments = 100; // Number of line segments to approximate each cubic Bezier curve
    float step = 1.0 / numSegments;

    for (int i = 0; i < numPoints - 3; i += 3) {
        float x0 = pointsX[i];
        float y0 = pointsY[i];
        float x1 = pointsX[i + 1];
        float y1 = pointsY[i + 1];
        float x2 = pointsX[i + 2];
        float y2 = pointsY[i + 2];
        float x3 = pointsX[i + 3];
        float y3 = pointsY[i + 3];

        for (float t = 0; t <= 1; t += step) {
            float x, y;
            cubicBezier(x0, y0, x1, y1, x2, y2, x3, y3, t, &x, &y);
            int pixel_x = (int)x;
            int pixel_y = (int)y;
            draw_tile(pixel_x, pixel_y, 1, 1, color);
        }
    }
}