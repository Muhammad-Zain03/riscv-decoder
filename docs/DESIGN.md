# Decoder Design Document

## Architecture Overview
The project is structurally divided across three components:
1. **Memory Subsystem (`memory.c`):** Responsible for parsing the binary hex files into a simulated, heap-allocated memory array with correct little-endian ordering.
2. **Decoder Logic (`decoder.c`):** Handles all bitwise manipulations, applying correct `struct` modeling and `enum` dispatching to decompose the 32-bit machine word.
3. **Execution (`main.c`):** Connects memory logic to decoder operations, handles strings, formats output, and actively guarantees zero memory leaks via proper `free()` calls.

## Sign Extension
Correctly decoding RISC-V immediate values relies on a unified `sign_extend` function, adapting varying length N-bit immediate fields up into 32-bit signed integers utilizing bitwise XORs and subtractions.