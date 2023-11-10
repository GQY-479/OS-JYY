#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int count = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      if (*fmt == 'd') {
        int num = va_arg(args, int);
        putch(num + '0');  // convert the integer to its ascii character representation for printing.

        count++;
      } else if (*fmt == 's') {
        char *str = va_arg(args, char *);
        while (*str) {
          putch(*str);
          str++;
          count++;
        }
        count++;
      }
    } else {
      putch(*fmt);

      count++;
    }
    fmt++;
  }

  va_end(args);
  return count;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
