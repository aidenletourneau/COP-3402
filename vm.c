#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRAY_SIZE 500
#define INSTRUCTION_SIZE 3

unsigned int pas[ARRAY_SIZE];

/**********************************************/
/* Find base L levels down */
/* */
/**********************************************/
int base(int BP, int L){
    int arb = BP; // arb = activation record base
    while ( L > 0){ //find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}



typedef struct InstructionRegister{
    int OP;
    int L;
    int M;
} InstructionRegister;



int main(int argc, char **argv){
    
    // registers
    unsigned int SP = 500;
    unsigned int BP = SP - 1;
    unsigned int PC = 10;
    InstructionRegister IR;
    


    // first we need to load the program from the input file into the PAS
    FILE * fptr = fopen("program.asc", "r");
    unsigned int offset = PC;
    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        printf("Reading line: %s\n", line);


        pas[offset] = atoi(strtok(line, " "));
        pas[offset + 1] = atoi(strtok(NULL, " "));
        pas[offset + 2] = atoi(strtok(NULL, " "));
        

        printf("Line stored at pas[%u] %u %u %u\n",offset,  pas[offset],  pas[offset + 1],  pas[offset + 2]);
        offset += 3;
    }
   
    // fetch execute cycle
    while(1)

    //instruction fetch
    IR.OP = pas[PC];
    IR.L = pas[PC+1];
    IR.M = pas[PC+2];
    PC += 3;


    // switch statement for instruction decode
    switch(IR.OP){
        case 1: // LIT
            SP -= 1;
            pas[SP] = IR.M;
            break;

        case 2: // RTN
            SP = BP + 1;
            BP = pas[SP - 2];
            PC = pas[SP - 3];
            break;

        case 3: // LOD
            SP -= 1;
            pas[SP] = pas[base(BP, IR.L) - IR.M];
            break;

        case 4: // STO
            pas[base(BP, IR.L) - IR.M] = pas[SP];
            SP += 1;
            break;

        case 5: // CAL
            break;
        case 6: // INC
            break;
        case 7: // JMP
            break;
        case 8: // JPC 
            break;
        case 9: // SYSCALL

            switch (IR.M){
                case 1:
                    
                    break;
            }
            break;
    }
    






    return 1;
}