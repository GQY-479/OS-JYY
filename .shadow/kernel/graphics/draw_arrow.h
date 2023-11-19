#include "draw_tile.h"
#include "draw_line.h"

void draw_arrow(int x1, int y1, int x2, int y2, int color) {
    draw_line(x1, y1, x2, y2, color);
    int dx = x2 - x1, dy = y2 - y1;
    int x3 = x2 - dx / 3, y3 = y2 - dy / 3;
    int x4 = x3 - dx / 3, y4 = y3 - dy / 3;
    draw_line(x2, y2, x4, y4, color);
    draw_line(x2, y2, x4 + dy / 3, y4 - dx / 3, color);
}