#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>
#include "image_data.h"
#include "./graphics/draw_tile.h"
#include "./graphics/draw_line.h"

#define SIDE 8

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



// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  get_w_h();

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  if(0){
    unsigned char* image_data = hair_flowing;
    int image_width = 2268;
    int image_height = 1200;
    draw_image(image_data, image_width, image_height);
  }
  
  draw_line(0, 0, w, h, 0xffffff, 0, SIDE, 1);
  draw_line(0, h, w, 0, 0xffffff, 0, 1, 1);
  draw_line(0, 0, w-100, h-100, 0xff0000, 0, 8, 2);
  draw_line(50, 50, 300, 600, 0x0000ff, 1, 8, 3);
  // splash();

  puts("Press any key to see its key code...\n");
  while (1) {
    print_key();
  }
  return 0;
}
