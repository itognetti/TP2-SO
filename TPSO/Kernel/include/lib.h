#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#define MOD_INCREMENT(n, total) (n) = ((n) + 1) % (total);
#define MOD_DECREMENT(n, total) n--;                                \
                                if((n) < 0)                         \
                                    n = ((total) + (n)) % (total);  \
                                else                                \
                                    n = (n) % (total);

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int hexToString(uint64_t number, char * buffer, int length);

#endif