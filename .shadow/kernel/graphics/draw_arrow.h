#include "draw_tile.h"
#include "draw_line.h"
// #include <math.h>

#define sqrt(x) __builtin_sqrt(x)

void draw_arrow(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side) {
    draw_line(x1, y1, x2, y2, color, bold, pixel_side, 1);
    int dx = x2 - x1, dy = y2 - y1;
    dx = -dx; dy = -dy;
    // rotate 30 degree
    int dx1 = sqrt(3)/2 * dx - 1/2 * dy;
    int dy1 = 1/2 * dx + sqrt(3)/2 * dy;
    int dx2 = sqrt(3)/2 * dx + 1/2 * dy;
    int dy2 = -1/2 * dx + sqrt(3)/2 * dy;
    
    // float len = sqrt(dx*dx + dy*dy);
    dx1 = dx1 / 4; dy1 = dy1 / 4;
    dx2 = dx2 / 4; dy2 = dy2 / 4;

    
    draw_line(x2, y2, x2+dx1, y2+dy1, color, bold, pixel_side, 1);
    draw_line(x2, y2, x2+dx2, y2+dy2, color, bold, pixel_side, 1);
}