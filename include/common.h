#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define EXTRACT_BITS(val, high, low) (((val) >> (low)) & ((1U << ((high) - (low) + 1)) - 1))

#define MEMORY_SIZE 65536 

#define OPCODE_MASK 0x7F

#endif // COMMON_H