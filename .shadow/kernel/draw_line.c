#include "draw_line.h"
#include "draw_tile.h"

// void draw_pixel(int x, int y) {
//     glBegin(GL_POINTS);
//     glVertex2i(x, y);
//     glEnd();
// }

void draw_pixel(int x, int y) {
    
}

void draw_line_bresenham(int x1, int y1, int x2, int y2, uint32_t color, int bold) {
    int dx, dy, i, e;
    int incx, incy, inc1, inc2;
    int x,y;

    dx = x2-x1;
    dy = y2-y1;

    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    incx = 1;
    if (x2 < x1) incx = -1;
    incy = 1;
    if (y2 < y1) incy = -1;
    x = x1; y = y1;
    if (dx > dy) {
        draw_tile(x, y, bold, bold, color);
        e = 2 * dy-dx;
        inc1 = 2*(dy-dx);
        inc2 = 2*dy;
        for (i=0; i<dx; i++) {
            if (e >= 0) {
                y += incy;
                e += inc1;
            }
            else
                e += inc2;
            x += incx;
            draw_tile(x, y, bold, bold, color);
        }

    } else {
        draw_tile(x, y, bold, bold, color);
        e = 2*dx-dy;
        inc1 = 2*(dx-dy);
        inc2 = 2*dx;
        for (i=0; i<dy; i++) {
            if (e >= 0) {
                x += incx;
                e += inc1;
            }
            else
                e += inc2;
            y += incy;
            draw_tile(x, y, bold, bold, color);
        }
    }
}

// DDA Line Drawing Algorithm
void draw_line_dda(int x1, int y1, int x2, int y2, uint32_t color, int bold) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float Xinc = dx / (float) steps;
    float Yinc = dy / (float) steps;
    float X = x1;
    float Y = y1;
    for (int i = 0; i <= steps; i++) {
        draw_pixel(X,Y);
        X += Xinc;
        Y += Yinc;
    }
}

// Midpoint Line Drawing Algorithm
void draw_line_midpoint(int x1, int y1, int x2, int y2, uint32_t color, int bold) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int change1 = 2 * dy;
    int change2 = 2 * (dy - dx);
    int x = x1;
    int y = y1;

    draw_tile(x, y, bold, bold, color);

    while (x < x2) {
        x++;
        if (d < 0) {
            d = d + change1;
        } else {
            y++;
            d = d + change2;
        }
        draw_tile(x, y, bold, bold, color);
    }
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color, int bold, int algorithm) {
    switch (algorithm) {
        case 1:
            draw_line_bresenham(x1, y1, x2, y2, color, bold);
            break;
        case 2:
            draw_line_dda(x1, y1, x2, y2, color, bold);
            break;
        case 3:
            draw_line_midpoint(x1, y1, x2, y2, color, bold);
            break;
        default:
            printf("Invalid algorithm choice. Please choose between 1 (Bresenham), 2 (DDA), or 3 (Midpoint).\n");
            break;
    }
}
