// #include <stdio.h>
// #include <math.h>
#include "draw_tile.h"
#include "draw_line.h"

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
    assert(sizeof(pointsX) == sizeof(pointsY));
    assert(numPoints >= 3 && numPoints % 2 == 1);

    for (int i = 0; i < numPoints; i++) {
        float x0 = pointsX[i];
        float y0 = pointsY[i];
        float x1 = pointsX[i + 1];
        float y1 = pointsY[i + 1];
        float x2 = pointsX[i + 2];
        float y2 = pointsY[i + 2];
		
		int pixel_x = x0, pixel_y = y0;
		int prev_pixel_x = x0, prev_pixel_y = y0;

        for (int j = 0; j <= numSegments; j++) {
            float t = j / numSegments;
            float x, y;
            quadraticBezier(x0, y0, x1, y1, x2, y2, t, &x, &y);
			
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
    assert(sizeof(pointsX) == sizeof(pointsY));
    assert(numPoints >= 4 && numPoints % 3 == 1);

    for (int i = 0; i < numPoints; i++) {
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
            float t = j / numSegments;
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