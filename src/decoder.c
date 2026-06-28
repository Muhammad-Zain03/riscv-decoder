#include "decoder.h"

static int32_t sign_extend(uint32_t val, int bit_width) {
    uint32_t sign_bit = 1U << (bit_width - 1);
    return (int32_t)((val ^ sign_bit) - sign_bit);
}

void decode_instruction(uint32_t raw, decoded_instr_t *out) {
    out->opcode = EXTRACT_BITS(raw, 6, 0);
    out->rd     = EXTRACT_BITS(raw, 11, 7);
    out->funct3 = EXTRACT_BITS(raw, 14, 12);
    out->rs1    = EXTRACT_BITS(raw, 19, 15);
    out->rs2    = EXTRACT_BITS(raw, 24, 20);
    out->funct7 = EXTRACT_BITS(raw, 31, 25);
    out->imm    = 0;

    switch (out->opcode) {
        case OP_I_TYPE:
        case OP_LOAD:
        case OP_JALR: {
            uint32_t raw_imm = EXTRACT_BITS(raw, 31, 20);
            out->imm = sign_extend(raw_imm, 12);
            break;
        }
        case OP_STORE: {
            uint32_t raw_imm = (EXTRACT_BITS(raw, 31, 25) << 5) | EXTRACT_BITS(raw, 11, 7);
            out->imm = sign_extend(raw_imm, 12);
            break;
        }
        case OP_BRANCH: {
            uint32_t raw_imm = (EXTRACT_BITS(raw, 31, 31) << 12) |
                               (EXTRACT_BITS(raw, 7, 7)   << 11) |
                               (EXTRACT_BITS(raw, 30, 25) << 5)  |
                               (EXTRACT_BITS(raw, 11, 8)  << 1);
            out->imm = sign_extend(raw_imm, 13);
            break;
        }
        case OP_LUI:
        case OP_AUIPC: {
            out->imm = raw & 0xFFFFF000; 
            break;
        }
        case OP_JAL: {
            uint32_t raw_imm = (EXTRACT_BITS(raw, 31, 31) << 20) |
                               (EXTRACT_BITS(raw, 19, 12) << 12) |
                               (EXTRACT_BITS(raw, 20, 20) << 11) |
                               (EXTRACT_BITS(raw, 30, 21) << 1);
            out->imm = sign_extend(raw_imm, 21);
            break;
        }
    }
}