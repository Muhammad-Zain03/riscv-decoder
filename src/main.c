#include "decoder.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

const char* get_mnemonic(decoded_instr_t *dec) {
    switch (dec->opcode) {
        case OP_R_TYPE:
            if (dec->funct3 == 0) return (dec->funct7 == 0x20) ? "sub" : "add";
            if (dec->funct3 == 1) return "sll";
            if (dec->funct3 == 2) return "slt";
            if (dec->funct3 == 3) return "sltu";
            if (dec->funct3 == 4) return "xor";
            if (dec->funct3 == 5) return (dec->funct7 == 0x20) ? "sra" : "srl";
            if (dec->funct3 == 6) return "or";
            if (dec->funct3 == 7) return "and";
            break;
        case OP_I_TYPE:
            if (dec->funct3 == 0) return "addi";
            if (dec->funct3 == 2) return "slti";
            if (dec->funct3 == 3) return "sltiu";
            if (dec->funct3 == 4) return "xori";
            if (dec->funct3 == 6) return "ori";
            if (dec->funct3 == 7) return "andi";
            if (dec->funct3 == 1) return "slli";
            if (dec->funct3 == 5) return (dec->funct7 == 0x20) ? "srai" : "srli";
            break;
        case OP_LOAD:
            if (dec->funct3 == 0) return "lb";
            if (dec->funct3 == 1) return "lh";
            if (dec->funct3 == 2) return "lw";
            if (dec->funct3 == 4) return "lbu";
            if (dec->funct3 == 5) return "lhu";
            break;
        case OP_STORE:
            if (dec->funct3 == 0) return "sb";
            if (dec->funct3 == 1) return "sh";
            if (dec->funct3 == 2) return "sw";
            break;
        case OP_BRANCH:
            if (dec->funct3 == 0) return "beq";
            if (dec->funct3 == 1) return "bne";
            if (dec->funct3 == 4) return "blt";
            if (dec->funct3 == 5) return "bge";
            if (dec->funct3 == 6) return "bltu";
            if (dec->funct3 == 7) return "bgeu";
            break;
        case OP_LUI:   return "lui";
        case OP_AUIPC: return "auipc";
        case OP_JAL:   return "jal";
        case OP_JALR:  return "jalr";
    }
    return "UNKNOWN";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hex_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    uint8_t *memory = calloc(MEMORY_SIZE, 1);
    if (!memory) return EXIT_FAILURE;

    int instr_count = load_hex_file(argv[1], memory, MEMORY_SIZE);
    if (instr_count < 0) {
        free(memory);
        return EXIT_FAILURE;
    }

    int valid = 0, unknown = 0;

    for (int i = 0; i < instr_count; i++) {
        uint32_t addr = i * 4;
        
        // Cast to uint32_t prevents signed integer overflow on bytes >= 0x80
        uint32_t raw_inst = ((uint32_t)memory[addr+3] << 24) | 
                            ((uint32_t)memory[addr+2] << 16) | 
                            ((uint32_t)memory[addr+1] << 8)  | 
                             (uint32_t)memory[addr+0];
                             
        decoded_instr_t dec;
        decode_instruction(raw_inst, &dec);

        const char *mnem = get_mnemonic(&dec);
        
p       rintf("0x%08X %08X | ", addr, raw_inst);

        if (mnem[0] == 'U' || raw_inst == 0xDEADBEEF) {
            printf("UNKNOWN\n");
            unknown++;
            continue;
        }

        valid++;
        switch (dec.opcode) {
            case OP_R_TYPE:
                printf("%-7s x%u, x%u, x%u\n", mnem, dec.rd, dec.rs1, dec.rs2);
                break;
            case OP_I_TYPE:
                if (dec.funct3 == 1 || dec.funct3 == 5) {
                    printf("%-7s x%u, x%u, %d\n", mnem, dec.rd, dec.rs1, dec.imm & 0x1F);
                } else {
                    printf("%-7s x%u, x%u, %d\n", mnem, dec.rd, dec.rs1, dec.imm);
                }
                break;
            case OP_LOAD:
            case OP_JALR:
                printf("%-7s x%u, %d(x%u)\n", mnem, dec.rd, dec.imm, dec.rs1);
                break;
            case OP_STORE:
                printf("%-7s x%u, %d(x%u)\n", mnem, dec.rs2, dec.imm, dec.rs1);
                break;
            case OP_BRANCH:
                printf("%-7s x%u, x%u, %d\n", mnem, dec.rs1, dec.rs2, dec.imm);
                break;
            case OP_LUI:
            case OP_AUIPC:
                printf("%-7s x%u, 0x%X\n", mnem, dec.rd, (dec.imm >> 12) & 0xFFFFF);
                break;
            case OP_JAL:
                printf("%-7s x%u, %d\n", mnem, dec.rd, dec.imm);
                break;
        }
    }

    free(memory); 
    return EXIT_SUCCESS;
}