#include "decoder.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 65536 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hex_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    uint8_t *memory = calloc(MEM_SIZE, 1);
    if (!memory) return EXIT_FAILURE;

    int instr_count = load_hex_file(argv[1], memory, MEM_SIZE);
    if (instr_count < 0) {
        free(memory);
        return EXIT_FAILURE;
    }

    printf("Addr       Hex      | Assembly\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < instr_count; i++) {
        uint32_t addr = i * 4;

        uint32_t raw_inst = (memory[addr+3] << 24) | 
                            (memory[addr+2] << 16) | 
                            (memory[addr+1] << 8)  | 
                             memory[addr+0];
                             
        decoded_instr_t decoded;
        decode_instruction(raw_inst, &decoded);

        printf("0x%08X %08X | ", addr, raw_inst);
        
        if (decoded.opcode == OP_I_TYPE && decoded.funct3 == 0x0) {
            printf("addi    x%u, x%u, %d\n", decoded.rd, decoded.rs1, decoded.imm);
        } else if (decoded.opcode == OP_R_TYPE && decoded.funct3 == 0x0 && decoded.funct7 == 0x00) {
            printf("add     x%u, x%u, x%u\n", decoded.rd, decoded.rs1, decoded.rs2);
        } else {
            printf("UNKNOWN\n");
        }
    }

    free(memory);
    return EXIT_SUCCESS;
}