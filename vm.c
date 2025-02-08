/*
Aiden Letourneau and Malik Fisher
University of Central Florida COP 3402
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ARRAY_SIZE 500
#define INSTRUCTION_SIZE 3
unsigned int pas[ARRAY_SIZE];

typedef struct InstructionRegister{
    unsigned int OP;
    unsigned int L;
    unsigned int M;
} InstructionRegister;

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

// function to print stack recursively
// this function isn't implementing any logic, its just used to print the stack the appropriate way
void printStack(unsigned int BP, unsigned int SP){
    
    if(BP == 499){ // reached bottom of stack
        for(int i = BP; i >= SP; i--){
            printf(" %u ", pas[i]);
        }
    }
    else if(SP > BP) printStack(pas[BP - 1] , SP); // handles CAL instructions
    
    else{ // not at the bottom yet so recursively call the next activation record
        printStack(pas[BP - 1] , BP+1);
        printf("|");
        for(int i = BP; i >= SP; i--){
            printf(" %u ", pas[i]);
        }
    }
}

int main(int argc, char **argv){
    
    // registers
    unsigned int SP = 500;
    unsigned int BP = SP - 1;
    unsigned int PC = 10;
    unsigned int eop = 1;
    InstructionRegister IR;


    // first we need to load the program from the input file into the PAS
    FILE * fptr = fopen(argv[1], "r");

    unsigned int offset = PC;
    char line[100];
    while(fgets(line, sizeof(line), fptr) != NULL){
        pas[offset] = atoi(strtok(line, " \t"));
        pas[offset + 1] = atoi(strtok(NULL, " \t"));
        pas[offset + 2] = atoi(strtok(NULL, " \t"));
        offset += 3;
    }


    printf("%16s %-4s%-4s%-4s %s\n", " ", "PC", "BP", "SP", "Stack");
    printf("%16s %-4u%-4u%-4u\n\n","Initial Values: ", PC, BP, SP);
    // fetch execute cycle
    while(eop){
        
        // instruction fetch
        IR.OP = pas[PC];
        IR.L = pas[PC+1];
        IR.M = pas[PC+2];
        PC += INSTRUCTION_SIZE;


        // General Note: SP -= 1 is "raising the stack pointer" and SP += 1 is lowering, this is because our stack is growing down


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
                pas[SP - 3] = PC; // return address
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
                        SP += 1;
                        printf("%-4s %-2u %-8u","SYS", IR.L, IR.M);

                        break;

                    case 2:
                        SP = SP-1; 
                        printf("Please Enter an Integer: ");
                        scanf("%u", &pas[SP]);
                        printf("\n%-4s %-2u %-8u","SYS", IR.L, IR.M);

                        break;
                    case 3:
                        eop = 0;
                        printf("%-4s %-2u %-8u","SYS", IR.L, IR.M);
                        break;
                }
                break;
        }


        // print our registers and the stack after each command
        printf(" %-4u%-4u%-4u", PC, BP, SP);
        printStack(BP, SP); 
        printf("\n");
    }

    return 1;
}