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

//function just for debugging
void printStack(int BP, int SP){
    printf("   Stack: ");
    for(int i = BP; i >= SP; i--){
        printf(" %d ", pas[i]);
    }
    printf("\n");
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
    unsigned int eop = 1;
    InstructionRegister IR;
    


    // first we need to load the program from the input file into the PAS
    FILE * fptr = fopen("program.asc", "r");
    unsigned int offset = PC;
    char line[100];
    while(fgets(line, sizeof(line), fptr) != NULL){
        //printf("Reading line: %s\n", line);


        pas[offset] = atoi(strtok(line, " "));
        pas[offset + 1] = atoi(strtok(NULL, " "));
        pas[offset + 2] = atoi(strtok(NULL, " "));
        

        //printf("Line stored at pas[%u] %u %u %u\n",offset,  pas[offset],  pas[offset + 1],  pas[offset + 2]);
        offset += 3;
    }







    FILE* output = fopen("output.txt", "w");

    printf("%16s %-4s%-4s%-4s\n", " ", "PC", "BP", "SP", "Stack");
    printf("%16s %-4u%-4u%-4u\n","Initial Values: ", PC, BP, SP);


    
    // fetch execute cycle
    while(PC < offset){
        
        // instruction fetch
        IR.OP = pas[PC];
        IR.L = pas[PC+1];
        IR.M = pas[PC+2];
        PC += 3;
        printf("executing %u %u %u\n", IR.OP, IR.L, IR.M);

    

        // instruction execute
        switch(IR.OP){
            case 1: // LIT
                printf("LIT %u %u", IR.L, IR.M);
                SP -= 1;
                pas[SP] = IR.M;

                
               
                break;

            case 2: // OPR
                switch (IR.M){
                    case 0: // RTN
                        break;
                    case 1: // ADD
                        printf("ADD %u %u", IR.L, IR.M);
                        break;
                    case 2: // SUB
                        break;
                    case 3: // MUL
                        break;
                    case 4: // DIV
                        break;
                    case 5: // EQL
                        break;
                    case 6: // NEQ
                        break;
                    case 7: // LSS
                        break;
                    case 8: // LEQ
                        break;
                    case 9: // GTR
                        break;
                    case 10: // GEQ
                        break;
                }
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
                pas[SP - 1] = base(BP, IR.L); 
                pas[SP - 2] = BP;
                pas[SP - 3] = PC; 
                BP = SP - 1;
                PC = IR.M;
                break;

            case 6: // INC
                SP = SP - IR.M;
                break;

            case 7: // JMP
                PC = IR.M;
                break;

            case 8: // JPC 
                if(pas[SP] == 0) PC = IR.M;
                SP += 1;
                break;
            case 9: // SYSCALL

                switch (IR.M){
                    case 1:
                        putc(pas[SP], output); 
                        SP += 1;
                        break;
                    case 2:
                        SP = SP-1; 
                        //pas[SP] = getc();
                        break;
                    case 3:
                        eop = 0;
                        break;
                }

                break;
        }

        printStack(BP, SP);
    }





    return 1;
}