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
    
    for(int i = 500; i >= SP; i--){
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
        //printf("Reading line: %s", line);
        pas[offset] = atoi(strtok(line, " "));
        pas[offset + 1] = atoi(strtok(NULL, " "));
        pas[offset + 2] = atoi(strtok(NULL, " "));
        //printf("Line stored at pas[%u] %u %u %u\n\n",offset,  pas[offset],  pas[offset + 1],  pas[offset + 2]);
        offset += 3;
    }









    printf("%16s %-4s%-4s%-4s %s\n", " ", "PC", "BP", "SP", "Stack");
    printf("%16s %-4u%-4u%-4u\n","Initial Values: ", PC, BP, SP);
    // fetch execute cycle
    while(eop){
        
        // instruction fetch
        IR.OP = pas[PC];
        IR.L = pas[PC+1];
        IR.M = pas[PC+2];
        PC += 3;


        // instruction execute
        switch(IR.OP){
            case 1: // LIT
                
                SP -= 1;
                pas[SP] = IR.M;
                printf("%-4s %-2u %-8u","LIT", IR.L, IR.M);
                break;

            case 2: // OPR
                switch (IR.M){
                    case 0: // RTN
                        SP = BP + 1; 
                        BP = pas[SP - 2]; 
                        PC = pas[SP - 3];
                        printf("%-4s %-2u %-8u","RTN", IR.L, IR.M);
                        break;
                    case 1: // ADD
                        pas[SP + 1] = pas[SP + 1] + pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","ADD", IR.L, IR.M);

                        break;
                    case 2: // SUB
                        pas[SP + 1] = pas[SP + 1] - pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","SUB", IR.L, IR.M);

                        break;
                    case 3: // MUL
                        pas[SP + 1] = pas[SP + 1] * pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","MUL", IR.L, IR.M);

                        break;
                    case 4: // DIV
                        pas[SP + 1] = pas[SP + 1] / pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","DIV", IR.L, IR.M);

                        break;
                    case 5: // EQL
                        pas[SP + 1] = pas[SP + 1] == pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","EQL", IR.L, IR.M);

                        break;
                    case 6: // NEQ
                        pas[SP + 1] = pas[SP + 1] != pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","NEQ", IR.L, IR.M);

                        break;
                    case 7: // LSS
                        pas[SP + 1] = pas[SP + 1] < pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","LSS", IR.L, IR.M);

                        break;
                    case 8: // LEQ
                        pas[SP + 1] = pas[SP + 1] <= pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","LEQ", IR.L, IR.M);

                        break;
                    case 9: // GTR
                        pas[SP + 1] = pas[SP + 1] > pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","GTR", IR.L, IR.M);

                        break;
                    case 10: // GEQ
                        pas[SP + 1] = pas[SP + 1] >= pas[SP];
                        SP += 1;
                        printf("%-4s %-2u %-8u","GEQ", IR.L, IR.M);

                        break;
                }
                break;

            case 3: // LOD
                SP -= 1;
                pas[SP] = pas[base(BP, IR.L) - IR.M];
                printf("%-4s %-2u %-8u","LOD", IR.L, IR.M);

                break;

            case 4: // STO
                pas[base(BP, IR.L) - IR.M] = pas[SP];
                SP += 1;
                printf("%-4s %-2u %-8u","STO", IR.L, IR.M);

                break;

            case 5: // CAL
                pas[SP - 1] = base(BP, IR.L); // static link
                pas[SP - 2] = BP; // dynamic link
                pas[SP - 3] = PC; //return address
                BP = SP - 1;
                PC = IR.M;
                printf("%-4s %-2u %-8u","CAL", IR.L, IR.M);

                break;

            case 6: // INC
                SP = SP - IR.M;
                printf("%-4s %-2u %-8u","INC", IR.L, IR.M);

                break;

            case 7: // JMP
                PC = IR.M;
                printf("%-4s %-2u %-8u","JMP", IR.L, IR.M);

                break;

            case 8: // JPC 
                if(pas[SP] == 0) PC = IR.M;
                SP += 1;
                printf("%-4s %-2u %-8u","JPC", IR.L, IR.M);

                break;

            case 9: // SYSCALL
                switch (IR.M){
                    case 1:
                        printf("Output result is: %u\n", pas[SP]);
                        //putc(pas[SP], output); 
                        SP += 1;
                        printf("%-4s %-2u %-8u","SYS", IR.L, IR.M);

                        break;

                    case 2:
                        SP = SP-1; 
                        scanf("Please Enter an Integer: %u\n", &pas[SP]); // change this eventually
                        printf("%-4s %-2u %-8u","SYS", IR.L, IR.M);

                        break;
                    case 3:
                        eop = 0;
                        printf("%-4s %-2u %-8u","SYS", IR.L, IR.M);

                        break;
                }
                break;
        }

        printf(" %-4u%-4u%-4u", PC, BP, SP);
        printStack(BP, SP);

    
        
    }

    return 1;
}