#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <stdarg.h> // Needed for varying argument length

int strlen(char* str);
char* strcpy(const char* src, char* dst);
unsigned int strcmp(char* str1, char* str2);
unsigned int strcasecmp(char* str1, char* str2);
void itoa(int number, char* buf);
void printf(char* text, ...);
void printf_i(char* text, ...);

#endif // STRINGUTIL_H