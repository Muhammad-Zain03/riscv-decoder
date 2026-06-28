#include "decoder.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test_r_type() {
    decoded_instr_t dec;

    decode_instruction(0x00A28233, &dec);

    assert(dec.opcode == OP_R_TYPE);
    assert(dec.rd == 4);
    assert(dec.funct3 == 0);
    assert(dec.rs1 == 5);
    assert(dec.rs2 == 10);
    assert(dec.funct7 == 0);
    
    printf("R-Type decode test passed.\n");
}

void test_i_type() {
    decoded_instr_t dec;

    decode_instruction(0x00500113, &dec);
    
    assert(dec.opcode == OP_I_TYPE);
    assert(dec.rd == 2);
    assert(dec.funct3 == 0);
    assert(dec.rs1 == 0);
    assert(dec.imm == 5);
    
    printf("I-Type decode test passed.\n");
}

void test_branch() {
    decoded_instr_t dec;

    decode_instruction(0xFE209CE3, &dec);
    
    assert(dec.opcode == OP_BRANCH);
    assert(dec.funct3 == 1);
    assert(dec.rs1 == 1);
    assert(dec.rs2 == 2);

    assert(dec.imm == -8); 
    
    printf("Branch decode test passed.\n");
}

int main(void) {
    printf("Running Decoder Unit Tests...\n");
    test_r_type();
    test_i_type();
    test_branch();
    printf("All tests passed!\n");
    
    return EXIT_SUCCESS;
}