#include "draw_tile.h"
#include "draw_line.h"

#define sqrt3 1.732

void draw_arrow(int x1, int y1, int x2, int y2, uint32_t color, int bold, int pixel_side, float scale) {
    draw_line(x1, y1, x2, y2, color, bold, pixel_side, 1);
    int dx = x2 - x1, dy = y2 - y1;
    dx = dx * scale; dy = dy * scale;
    dx = -dx; dy = -dy;
    // rotate 30 degree
    int dx1 = sqrt3*0.5 * dx - 0.5 * dy;
    int dy1 = 0.5 * dx + sqrt3*0.5 * dy;
    int dx2 = sqrt3*0.5 * dx + 0.5 * dy;
    int dy2 = -0.5 * dx + sqrt3*0.5 * dy;
    
    draw_line(x2, y2, x2+dx1, y2+dy1, color, bold, pixel_side, 1);
    draw_line(x2, y2, x2+dx2, y2+dy2, color, bold, pixel_side, 1);
}