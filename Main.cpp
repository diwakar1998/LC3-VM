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

/*This method takes the operand if the add instruction
is a immediate type and extends the 5 bit number to
16 bits..*/
uint16_t sign_extend(uint16_t x,int bit_count){
    if((x >> (bit_count -1)) & 1){
        x |= (0xFFFF << bit_count);
    }
    return x;
}

//Updates the flag bits based on the result of instruction
void update_flags(uint16_t r)
{
    if(reg[r] == 0){
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15)
    {
        reg[R_COND] = FL_NEG;
    }
    else{
        reg[R_COND] = FL_POS;
    }
}


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
            {
                //Destination register bits are obtained
                uint16_t DR=(instr >> 9) & 0x7;
                //First operand SR1 bits obtained
                uint16_t SR1=(instr >> 6) & 0x7;
                //Checks if the instruction is immediate mode
                uint16_t imm_flag = (instr >> 5) & 0x1;
                if(imm_flag){
                    /* instr & 0x1F takes the number bits and sends 
                    them to sign_extend where 5 bit number is 
                    converted to 16 bit equivalent without changing
                    the original value*/
                    uint16_t imm5 = sign_extend(instr & 0x1F , 5);
                    reg[DR] = reg[SR1] + imm5;
                }
                else{
                    //isolates second operand bits
                    uint16_t SR2=instr & 0x7;
                    /*computes additon and sends the value to 
                    destination register*/
                    reg[DR]=reg[SR1]+reg[SR2];
                }
            update_flags(DR);                
            }
            break;
        case OP_AND:
            {
                //Destination register bits are obtained
                uint16_t DR=(instr >> 9) & 0x7;
                //First operand SR1 bits obtained
                uint16_t SR1=(instr >> 6) & 0x7;
                //Checks if the instruction is immediate mode
                uint16_t imm_flag = (instr >> 5) & 0x1;
                if(imm_flag){
                    /* instr & 0x1F takes the number bits and sends 
                    them to sign_extend where 5 bit number is 
                    converted to 16 bit equivalent without changing
                    the original value*/
                    uint16_t imm5 = sign_extend(instr & 0x1F , 5);
                    reg[DR] = reg[SR1] & imm5;
                }
                else{
                    //isolates second operand bits
                    uint16_t SR2=instr & 0x7;
                    /*computes AND of both SR's and sends 
                    the value to destination register*/
                    reg[DR]=reg[SR1] & reg[SR2];
                }
            update_flags(DR); 
            }
            break;
        case OP_NOT:
            {
                //Destination Register
                uint16_t DR = (instr >> 9)& 0x7;
                //Source Register
                uint16_t SR = (instr >> 6)& 0x7;
                //Not of SR stored in DR
                reg[DR] = ~reg[SR];
                update_flags(DR);
            }
            break;

        case OP_BR:
            {

            }

        case OP_JMP:
            {
                /* ALSO HANDLES RET*/
                //Base Register 
                uint16_t BR = (instr >> 6) & 0x7;

                reg[R_PC] = reg[BR];
            }
            break;
        case OP_JSR:
            {
                uint16_t flag = (instr >> 11) & 0x1;
                reg[R_R7] = reg[R_PC];
                if(flag == 0){
                    /*JSSR  --> look in the specification sheet*/
                    //Base Register 
                    uint16_t BR = (instr >> 6) & 0x7;
                    reg[R_PC] = reg[BR];
                }
                else{
                    //11 bits are extended to 16 bits 
                    uint16_t pc_offset = sign_extend(instr & 0x7FF,11);
                    reg[R_PC] += pc_offset; 
                }
            }
            break;
        case OP_LD:
            {
                //Destination Register is obtained
                uint16_t DR = (instr >> 9) & 0x7;
                //9 bits are extended to 16 bits 
                uint16_t pc_offset9 = sign_extend(instr & 0x1FF,9);
                //Operation is loading a specific value from memory
                reg[DR] = mem_read(reg[R_PC]+pc_offset9);
                update_flags(DR);
            }
        case OP_LDI:
            {
                //Destination Register is obtained
                uint16_t DR = (instr >> 9) & 0x7;
                //9 bits are extended to 16 bits 
                uint16_t pc_offset9 = sign_extend(instr & 0x1FF,9);
                /*Read value of pcoffset+PC--> which is a addres location
                then read the value in that address location*/
                reg[DR] = mem_read(mem_read(reg[R_PC]+pc_offset9));
                update_flags(DR);
            }
            break;
        case OP_LDR:
            {
                //Destination Register is obtained
                uint16_t DR = (instr >> 9) & 0x7;
                //BaseRegister
                uint16_t BR = (instr >> 6) & 0x7;
                //Offset6 value to add to BR 
                uint16_t offset6 = sign_extend(instr & 0x3F,6);
                //Actual operation
                reg[DR] = mem_read(reg[BR]+offset6);
                update_flags(DR);
                
            }
            break;
        case OP_LEA:
            {
                //9 bits are extended to 16 bits 
                uint16_t pc_offset9 = sign_extend(instr & 0x1FF,9);
                //DR 
                uint16_t DR = (instr >> 9) & 0x7;
                reg[DR] = reg[R_PC]+pc_offset9;
                update_flags(DR);
            }   
            break;
        case OP_ST:
            {
                //Source register bits obtained
                uint16_t SR = (instr >> 9) & 0x7;
                uint16_t pc_offset9 = sign_extend(instr & 0x1FF,9);
                mem_write(reg[R_PC]+pc_offset9), reg[SR]);
            }
            break;
        case OP_STI:
            {
                //Source register bits obtained
                uint16_t SR = (instr >> 9) & 0x7;
                uint16_t pc_offset9 = sign_extend(instr & 0x1FF,9);
                mem_write(mem_read(reg[R_PC]+pc_offset9) , reg[SR]);
            }
            
            break;
        case OP_STR:
            //;-) tried to fit the code in one line i was lazy
            mem_write(reg[((instr >> 6)&0x7)] + sign_extend(instr & 0x3F,6)) ,reg[(instr >> 9)&0x7]);
            break;
        case OP_TRAP:
            {

            }
            break;
        case OP_RES:
            abort();
        case OP_RTI:
            abort();
        default:
        
            break;
        }
    }
}