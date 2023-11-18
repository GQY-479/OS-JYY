#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void print_int(int num) {
  if (num == 0) {
    putch('0');
    return;
  }

  if (num < 0) {
    putch('-');
    num = -num;
  }

  char digits[10];
  int i = 0;

  while (num > 0) {
    digits[i] = num % 10 + '0';
    num /= 10;
    i++;
  }

  for (int j = i - 1; j >= 0; j--) {
    putch(digits[j]);
  }
}

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  int count = 0;
  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      if (*fmt == 'd') {
        int num = va_arg(args, int);
        print_int(num);
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
