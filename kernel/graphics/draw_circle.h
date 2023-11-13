#ifndef DRAW_CIRCLE_H_
#define DRAW_CIRCLE_H_

// #include <math.h>
#include "draw_tile.h"

void draw_symmetric_points(int x_center, int y_center, int x, int y, uint32_t color) {
    draw_tile(x_center + x, y_center - y, 1, 1, color);
    draw_tile(x_center + y, y_center - x, 1, 1, color);
    draw_tile(x_center + y, y_center + x, 1, 1, color);
    draw_tile(x_center + x, y_center + y, 1, 1, color);
    draw_tile(x_center - x, y_center + y, 1, 1, color);
    draw_tile(x_center - y, y_center + x, 1, 1, color);
    draw_tile(x_center - y, y_center - x, 1, 1, color);
    draw_tile(x_center - x, y_center - y, 1, 1, color);
}

void draw_symmetric_ellipse_points(int x_center, int y_center, int x, int y, uint32_t color) {
    draw_tile(x_center + x, y_center - y, 1, 1, color);
    draw_tile(x_center - x, y_center - y, 1, 1, color);
    draw_tile(x_center - x, y_center + y, 1, 1, color);
    draw_tile(x_center + x, y_center + y, 1, 1, color);
}

void draw_circle(int x_center, int y_center, int radius, uint32_t color) {
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;

    draw_symmetric_points(x_center, y_center, x, y, color);

    while (x < y) {
        x++;

        // Midpoint is inside or on the perimeter
        if (p <= 0) {
            p = p + 4 * x + 6;
        }
        // Midpoint is outside the perimeter
        else {
            y--;
            p = p + 4 * (x - y) + 10;
        }

        draw_symmetric_points(x_center, y_center, x, y, color);

        if (x >= y) {
            break;
        }
    }
}

void draw_ellipse(int x_center, int y_center, int a, int b, uint32_t color) {
    int x = 0;
    int y = b;

    // Initial decision parameter of region 1
    int d1 = b * b - a * a * b + a * a / 4;

    draw_symmetric_ellipse_points(x_center, y_center, x, y, color);

    // Region 1
    while (a * a * (y - 0.5) > b * b * (x + 1)) {
        if (d1 < 0) {
            d1 += b * b * (2 * x + 3);
        } else {
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            y--;
        }
        x++;

        // Draw points in region 1
        draw_symmetric_ellipse_points(x_center, y_center, x, y, color);
    }

    // Region 2
    int d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
    while (y > 0) {
        if (d2 < 0) {
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
        } else {
            d2 += a * a * (-2 * y + 3);
        }
        y--;

        // Draw points in region 2
        draw_symmetric_ellipse_points(x_center, y_center, x, y, color);
    }
}

#endif
