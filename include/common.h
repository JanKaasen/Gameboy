#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef uint16_t address;

#define MEMORY_SIZE 0x10000
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define TILES_PER_ROW 32
#define TILES_PER_COL 32
#define CPU_FREQUENCY 4194304

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define UNUSED(x) ((void) (x))

#endif
