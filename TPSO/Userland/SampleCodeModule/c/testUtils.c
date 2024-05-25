#include <testUtils.h>

static uint32_t z_seed = 362436069;
static uint32_t w_seed = 521288629;

uint32_t generateRandomUint() {
    z_seed = 36969 * (z_seed & 65535) + (z_seed >> 16);
    w_seed = 18000 * (w_seed & 65535) + (w_seed >> 16);
    return (z_seed << 16) + w_seed;
}

uint32_t generateUniformRandom(uint32_t max) {
    uint32_t randomUint = generateRandomUint();
    return (randomUint + 1.0) * 2.328306435454494e-10 * max;
}

//Verifica si un bloque de memoria de tamaño size a partir de start contiene el valor value
uint8_t checkMemory(void *start, uint8_t value, uint32_t size) {
    uint8_t *current = (uint8_t *)start;

    for (uint32_t i = 0; i < size; i++, current++) {
        if (*current != value) {
            return 0;
        }
    }
    return 1;
}

void bussyWait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endlessLoop() {
  while (1)
    ;
}

void endlessLoopPrint(uint64_t wait) {
  int64_t pid = getPid();

  while (1) {
    printf("%d ", pid);
    bussyWait(wait);
  }
}

int64_t satoi(const char *str) {
    uint64_t index = 0;
    int64_t result = 0;
    int8_t sign = 1;

    if (str == NULL) {
        return 0;
    }

    if (str[index] == '-') {
        index++;
        sign = -1;
    }

    for (; str[index] != '\0'; ++index) {
        if (str[index] < '0' || str[index] > '9') {
            return 0;
        }
        result = result * 10 + (str[index] - '0');
    }

    return result * sign;
}