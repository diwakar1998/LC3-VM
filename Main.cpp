//65536 memory locations
#include<iostream>

uint16_t memory[UINT16_MAX];


//Register Enum
enum{
    R_R0=0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,//programcounter
    R_COND,//condition flags register
    R_COUNT
};

uint16_t reg[R_COUNT];

//OP-Codes
enum{
    OP_BR = 0,  //BRANCH
    OP_ADD,     //ADD
    OP_LD,      //LOAD
    OP_ST,      //STORE
    OP_JSR,     //JUMP REGISTER
    OP_AND,     //BITWISE AND
    OP_LDR,     //LOAD REGISTER
    OP_STR,     //STORE REGISTER
    OP_RTI,     //UNUSED
    OP_NOT,     //BITWISE NOT
    OP_LDI,     //LOAD INDIRECT
    OP_STI,     //STORE INDIRECT
    OP_JMP,     //JUMP
    OP_RES,     //RESERVED(NOT USED)
    OP_LEA,     //LOAD EFFECTIVE ADDRESS
    OP_TRAP     //EXECUTE TRAP
};

//Flags
enum{
    FL_POS = 1 << 0, //Positive flag
    FL_ZRO = 1 << 1, //Zero flag
    FL_NEG = 1 << 2, //Negative flag
};




int main(){
}