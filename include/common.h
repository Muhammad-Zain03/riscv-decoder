#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define EXTRACT_BITS(val, msb, lsb) (((val) >> (lsb)) & ((1U << ((msb) - (lsb) + 1)) - 1))

#define OPCODE_MASK 0x7F

#endif