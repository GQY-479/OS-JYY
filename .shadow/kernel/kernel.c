#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>
#include "image_data.h"
#include "./graphics/draw_tile.h"
#include "./graphics/draw_line.h"
#include "./graphics/draw_circle.h"
#include "./graphics/draw_curve.h"
#include <time.h>

#define SIDE 3

static int w, h;  // Screen size

// Define a macro to map keycodes to key names
#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = { AM_KEYS(KEYNAME) };

static inline void puts(const char *s) {
  for (; *s; s++) putch(*s);
}

// This function prints the key that was pressed on the keyboard
void print_key() {
  // Create an instance of the AM_INPUT_KEYBRD_T struct and initialize the keycode to AM_KEY_NONE
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  
  // Read the keyboard input and store it in the event struct
  ioe_read(AM_INPUT_KEYBRD, &event);
  
  // Check if a key was pressed and if it is a keydown event
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }

  // Check if the ESC key was pressed and call halt() to exit
  if (event.keycode == AM_KEY_ESCAPE && event.keydown) {
    halt(1);
  }
}

void get_w_h(){
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;
}
// draw a tile(w*h) with color
#ifndef DRAW_TILE_H_
#define DRAW_TILE_H_
static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}
#endif

void splash() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;

  for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if ((x & 1) ^ (y & 1)) {
        draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
      }
    }
  }
}


// The (0,0) is at the top-left corner of the screen
// and the order of rgb is actually bgr. https://blog.csdn.net/weixin_40437029/article/details/117530796
// This function display a half-decoded BMP image
void draw_image(const unsigned char* image_data, int image_width, int image_height) {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;

  int pixel_size = 3;

  // Calculate the scaling factors
  float scale_x = (float)w / image_width;
  float scale_y = (float)h / image_height;

  // Iterate over each pixel in the new grid
  for (int y = h-1; y >= 0; y--) {
    for (int x = 0; x < w; x++) {
      // Calculate the corresponding pixel position in the original grid
      int original_x = (int)(x / scale_x);
      int original_y = (int)((h-y-1) / scale_y);

      // Get the RGB values from the original image data
      unsigned char b = image_data[(original_y * image_width + original_x) * pixel_size];
      unsigned char g = image_data[(original_y * image_width + original_x) * pixel_size + 1];
      unsigned char r = image_data[(original_y * image_width + original_x) * pixel_size + 2];

      // Combine the RGB values into a single color value
      uint32_t color = (r << 16) | (g << 8) | b;

      // Draw the pixel on the screen
      draw_tile(x, y, 1, 1, color);
    }
  }
}


#define PI 3.14159265359

// float sine(float x) {
//     float result = x;
//     float numerator = x * x * x;
//     float denominator = 6.0;
//     int sign = -1;

//     for (int i = 3; i <= 15; i += 2) {
//         result += sign * (numerator / denominator);
//         numerator *= x * x;
//         denominator *= (i + 1) * (i + 2);
//         sign *= -1;
//     }

//     return result;
// }

void generate_waveform(int w, int h, float* pointsX, float* pointsY, int numPoints) {
    float stepX = (float)w / (numPoints - 1);
    // float amplitude = h / 2.0;

    for (int i = 0; i < numPoints; i++) {
        pointsX[i] = i * stepX;
        // float angle = pointsX[i] * (2.0 * PI / w);
        // pointsY[i] = amplitude * sine(angle);
        pointsY[i] = h * ((int)rand() % 1000 / (float)1000);
    }

    printf("pointsX = {");
    for (int i = 0; i < numPoints; i++) {
        printf("%f, ", pointsX[i]);
    }
    printf("}\n");

    printf("pointsY = {");
    for (int i = 0; i < numPoints; i++) {
        printf("%f, ", pointsY[i]);
    }
    printf("}\n");
}

// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  get_w_h();
  printf("Screen size: %d x %d\n", w, h);

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  if(0){
    unsigned char* image_data = hair_flowing;
    int image_width = 2268;
    int image_height = 1200;
    draw_image(image_data, image_width, image_height);
  }
  
  if(0){
    draw_line(0, 0, w, h, 0xffffff, 0, SIDE, 1);
    draw_line(0, h, w, 0, 0xffffff, 0, 1, 1);
    draw_line(0, 0, w-100, h-100, 0xff0000, 0, 8, 2);
    draw_line(50, 50, 300, 600, 0x0000ff, 1, 8, 3);
  }

  if(0){
    draw_circle(w/2, h/2, 200, 0xff0000);
    draw_ellipse(w/2, h/2, 200, 100, 0x00ff00);
  }

  if (1){
        // Example usage
    // int x0 = 100, y0 = h-100;   // Start point
    // int x1 = w/2, y1 = 50;    // Control point
    // int x2 = w, y2 = h;   // End point
    int numSegments = 100; // Number of line segments to approximate the curve
    uint32_t color = 0xFF0000; // Red color
    int bold = 1;
    int pixel_side = SIDE;

    int numPoints = 100;  // Number of control points

    float pointsX[numPoints];
    float pointsY[numPoints];

    numPoints = 3;
    generate_waveform(w, h, pointsX, pointsY, numPoints);
    // draw_feature_graphics(pointsX, pointsY, numPoints, color, bold, pixel_side);
    drawQuadraticBezier(pointsX, pointsY, numPoints, numSegments, color, bold, pixel_side);

    // draw_line(x0, y0, x1, y1, color+0x00FF00, 0, 1, 1);
    // draw_line(x1, y1, x2, y2, color+0x00FF00, 0, 1, 1);
    

    // int x0 = 100, y0 = 100;   // Start point
    // int x1 = 150, y1 = 50;    // Control point 1
    // int x2 = 250, y2 = 150;   // Control point 2
    numPoints = 4;
    generate_waveform(w, h, pointsX, pointsY, numPoints);
    // int x3 = 300, y3 = 100;   // End point
    color = 0x00FF00; // Green color

    draw_feature_graphics(pointsX, pointsY, numPoints, color, bold, pixel_side);
    // drawCubicBezier(pointsX, pointsY, numPoints, numSegments, color, bold, pixel_side);
    // draw_line(x2, y2, x3, y3, color+0x0000FF, 0, 1, 1);


    // float pointsX[] = {100, w/2, w, 300, 350, 400, 450, 500}; // X-coordinates of control points
    // float pointsY[] = {h, 50, h/2, 100, h/3, 50, 300, 100}; // Y-coordinates of control points
    // int numPoints = sizeof(pointsX) / sizeof(pointsX[0]);
    numPoints = 16;
    generate_waveform(w, h, pointsX, pointsY, numPoints);
    color = 0x0000FF; // Blue color
    // draw_feature_graphics(pointsX, pointsY, numPoints, color, bold, pixel_side);
    drawCubicBezier(pointsX, pointsY, numSegments, numPoints, color, bold, pixel_side);
  }
  

  // splash();

  puts("Press any key to see its key code...\n");
  while (1) {
    print_key();
  }
  return 0;
}
