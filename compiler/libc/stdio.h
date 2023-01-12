#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H
#include <stdint.h>
#include <errno.h>

int puts(const char* buffer);
// Unsafe Function! Use gets_s() instead!
char* gets(char* output);
char* gets_s(char* output, size_t output_len);

char getc();

#endif