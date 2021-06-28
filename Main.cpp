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




int main(int argc, const char* argv[]){

    //Handling the inpput arguments
    if(argc <2){
        //Usage String
        printf("lc3 [image-file1] ...\n");
        exit(2);
    }
    //If correct input given then this code runs
    for (int i=1;i<argc;++i){
        if(!read_image(argv[i])){
            printf("Failed to load image: %s\n",argv[i]);
            exit(1);
        }
    }
    //{Setup ,12};


    /*Set PC start point to 0x3000 
    which is the default value*/
    enum {PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running =1;
    while(running){

        //Fetch the instruction from memory
        uint16_t instr = mem_read(reg[R_PC]++);

        /*Right Shift the left 4 bits to first 4 place
        thereby leaving only opcode in the op variable*/
        uint16_t op = instr >> 12;

        //required operation is performed based on opcode
        switch (op)
        {
        case OP_ADD:
        case OP_AND:
        case OP_NOT:
        case OP_BR:
        case OP_JMP:
        case OP_JSR:
        case OP_LD:
        case OP_LDI:
        case OP_LDR:
        case OP_LEA:
        case OP_ST:
        case OP_STI:
        case OP_STR:
        case OP_TRAP:
        case OP_RES:
        case OP_RTI:
        default:
        
            break;
        }
    }
}