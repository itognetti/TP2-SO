#ifndef STRING_H
#define STRING_H

int strlen(char *string);
int strcmp(const char *str1, const char *str2);
char * reverse(char* str, int length);
char * dateConverter(int num, char* str);
char * itoa(int num, char* str);
int strncpy(char * dest, const char * src, unsigned int n);
#endif