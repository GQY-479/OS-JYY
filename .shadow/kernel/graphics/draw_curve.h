// #include <stdio.h>
// #include <math.h>
#include "draw_tile.h"
#include "draw_line.h"
#include "draw_arrow.h"

// draw feature graphics
void draw_feature_graphics(float* pointsX, float* pointsY, int numPoints, uint32_t color, int bold, int pixel_side){
    for (int i = 0; i < numPoints-1; i++) {
        int x0 = pointsX[i];
        int y0 = pointsY[i];
        int x1 = pointsX[i + 1];
        int y1 = pointsY[i + 1];
        draw_line(x0, y0, x1, y1, color, bold, pixel_side, 1);
    }
}

void draw_feature_vector(float* pointsX, float* pointsY, float* slopesX, float* slopesY, int numPoints, uint32_t color, int bold, int pixel_side){
    for (int i = 0; i < numPoints; i += 1) {
        int x0 = pointsX[i];
        int y0 = pointsY[i];
        int x1 = x0 + slopesX[i] * 0.15;
        int y1 = y0 + slopesY[i] * 0.15;

        draw_arrow(x0, y0, x1, y1, color, bold, pixel_side, 0.2);
    }
}


// Function to calculate the quadratic Bezier curve point
void quadraticBezier(float x0, float y0, float x1, float y1, float x2, float y2, float t, float* x, float* y) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float ut = u * t;

    *x = uu * x0 + 2 * ut * x1 + tt * x2;
    *y = uu * y0 + 2 * ut * y1 + tt * y2;
}

void drawQuadraticBezier(float* pointsX, float* pointsY, int numPoints, int numSegments, uint32_t color, int bold, int pixel_side) {
    assert(numPoints >= 3 && numPoints % 2 == 1);

    for (int i = 0; i < numPoints-2; i += 2) {
        float x0 = pointsX[i];
        float y0 = pointsY[i];
        float x1 = pointsX[i + 1];
        float y1 = pointsY[i + 1];
        float x2 = pointsX[i + 2];
        float y2 = pointsY[i + 2];
		
		int pixel_x = x0, pixel_y = y0;
		int prev_pixel_x = x0, prev_pixel_y = y0;

        for (int j = 0; j <= numSegments; j++) {
            float t = j / (float)numSegments;
            float x, y;
            quadraticBezier(x0, y0, x1, y1, x2, y2, t, &x, &y);
			
            prev_pixel_x = pixel_x;
            prev_pixel_y = pixel_y;
            pixel_x = (int)x;
            pixel_y = (int)y;
			
            if(j > 0){
                // last parameter i shows that we use the Bresenham algorithm to draw a straight line.
                draw_line(prev_pixel_x, prev_pixel_y, pixel_x, pixel_y, color, bold, pixel_side, 1);
                // draw_tile(pixel_x, pixel_y, pixel_side, pixel_side, color);
            }
        }
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

// Function to draw a smooth Bezier curve using segment-wise cubic Bezier curves
void drawCubicBezier(float* pointsX, float* pointsY, int numPoints, int numSegments, uint32_t color, int bold, int pixel_side) {
    assert(numPoints >= 4 && numPoints % 3 == 1);

    for (int i = 0; i < numPoints-3; i += 3) {
        float x0 = pointsX[i];
        float y0 = pointsY[i];
        float x1 = pointsX[i + 1];
        float y1 = pointsY[i + 1];
        float x2 = pointsX[i + 2];
        float y2 = pointsY[i + 2];
        float x3 = pointsX[i + 3];
        float y3 = pointsY[i + 3];
		
		int pixel_x = x0, pixel_y = y0;
		int prev_pixel_x = x0, prev_pixel_y = y0;

        for (int j = 0; j <= numSegments; j++) {
            float t = j / (float)numSegments;
            float x, y;
            cubicBezier(x0, y0, x1, y1, x2, y2, x3, y3, t, &x, &y);
			
            prev_pixel_x = pixel_x;
            prev_pixel_y = pixel_y;
            pixel_x = (int)x;
            pixel_y = (int)y;
			
            if(j > 0){
                // last parameter i shows that we use the Bresenham algorithm to draw a straight line.
                draw_line(prev_pixel_x, prev_pixel_y, pixel_x, pixel_y, color, bold, pixel_side, 1);
            }
        }
    }
}


void cubicHermite(float x0, float y0, float m0x, float m0y,
                  float x1, float y1, float m1x, float m1y,
                  float t, float* x, float* y) {
    float h00 = 2 * t * t * t - 3 * t * t + 1; // calculate basis function 1
    float h10 = -2 * t * t * t + 3 * t * t; // calculate basis function 2
    float h01 = t * t * t - 2 * t * t + t; // calculate basis function 3
    float h11 = t * t * t - t * t; // calculate basis function 4

    // P=P1* h1(s)+ P2*h2(s)+ T1*h3(s)+T2*h4(s)
    *x = h00 * x0 + h10 * x1 + h01 * m0x + h11 * m1x; // multiply and sum all functions together to calculate the point on the curve
    *y = h00 * y0 + h10 * y1 + h01 * m0y + h11 * m1y;
}

// Function to draw a smooth Hermite curve using segment-wise cubic Hermite curves
void drawCubicHermite(float* pointsX, float* pointsY, float* slopesX, float* slopesY,
                      int numPoints, int numSegments, uint32_t color, int bold, int pixel_side) {

    for (int i = 0; i < numPoints - 1; i += 1) {
        float x0 = pointsX[i];
        float y0 = pointsY[i];
        float m0x = slopesX[i];
        float m0y = slopesY[i];
        float x1 = pointsX[i + 1];
        float y1 = pointsY[i + 1];
        float m1x = slopesX[i + 1];
        float m1y = slopesY[i + 1];

        int pixel_x = x0, pixel_y = y0;
        int prev_pixel_x = x0, prev_pixel_y = y0;

        for (int j = 0; j <= numSegments; j++) {
            float t = j / (float)numSegments;
            float x, y;
            cubicHermite(x0, y0, m0x, m0y, x1, y1, m1x, m1y, t, &x, &y);

            prev_pixel_x = pixel_x;
            prev_pixel_y = pixel_y;
            pixel_x = (int)x;
            pixel_y = (int)y;

            if (j > 0) {
                // Last parameter 1 shows that we use the Bresenham algorithm to draw a straight line.
                draw_line(prev_pixel_x, prev_pixel_y, pixel_x, pixel_y, color, bold, pixel_side, 1);
            }
        }
    }
}