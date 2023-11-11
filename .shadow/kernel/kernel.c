#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>
#include "image_data.h"

#define SIDE 16

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
    // Print a message indicating that a key was pressed
    puts("Key pressed: ");
    
    // Print the name of the key that was pressed using the key_names array
    puts(key_names[event.keycode]);
    
    // Print a newline character
    puts("\n");
  }

  // Check if the ESC key was pressed and call halt() to exit
  if (event.keycode == AM_KEY_ESCAPE && event.keydown) {
    halt(1);
  }
}

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

void draw_image(const unsigned char* image_data, int image_width, int image_height, int x, int y) {
  // Assuming a pixel is represented by 4 bytes (RGBA), adjust accordingly if needed
  int pixel_size = 4;
  
  // Create an array to store pixel data
  uint32_t pixels[image_width * image_height];

  // Convert the image data to pixel data
  for (int i = 0; i < image_width * image_height; i++) {
    pixels[i] = (image_data[i * pixel_size] << 16) | (image_data[i * pixel_size + 1] << 8) | image_data[i * pixel_size + 2];
  }

  // Create an instance of the AM_GPU_FBDRAW_T struct
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = image_width, .h = image_height, .sync = 1,
    .pixels = pixels,
  };

  // Draw the image on the screen
  ioe_write(AM_GPU_FBDRAW, &event);
}


// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  // splash();
  draw_image(hair_flowing, SIDE, SIDE, 100, 100);

  puts("Press any key to see its key code...\n");
  while (1) {
    print_key();
  }
  return 0;
}
