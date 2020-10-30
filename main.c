/*
 * Name: Kensal J Ramos
 *
 * Description: 
 *
 * This program implements a virtual machine (VM) known as a P-Machine (PM/0). The P-machine is a stack machine 
 * with two memory segments: the “stack,” which is organized as a data-stack which contains the data segment (first AR) and 
 * the stack to be used by the PM/0 CPU, and the “text”, which contains the instructions for the VM to execute. The PM/0 CPU 
 * has four registers to handle the stack and text segments: The registers are named base pointer (BP), stack pointer (SP), 
 * program counter (PC) and instruction register (IR). 
 *
 * This P-Machine's Instruction Set Architecture (ISA) has 22 instructions and the instruction format has four fields:
 * OP - Operation code
 * R - Refers to an index in register file (RF) array
 * L - Lexographical level 
 * M - Changes based on operation code. 
 * 
 * Required functionality:
 * 
 * ISA instruction implementation.
 * P-Machine split into 2 cycles: Fetch Cycle and Execute Cycle.
 *
 *
 * - Assigned by Professor Montagne as a project in COP 3402 at the University of Central Florida -
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

// Instruction struct
 struct instruction {

 	int op; // The operation code
 	int r; // A register to write into
 	int l; // Indicates the lexographical level or register in arithmetic
 	int m; // Changes depending on operator

 } ;

 void fetch(int *PC, int *SP, int *BP, struct instruction *IR, int *stack, int *RF);
 void execute(FILE *ofp, struct instruction *instArr, int *PC, int *SP, int *BP, struct instruction *IR, int *stack, int *RF);
 int base(int l, int base, int *stack);

 int main(void) {

 	int PC = 0; // Program counter 
 	int SP = MAX_STACK_HEIGHT; // Stack pointer - Points to top of stack 
 	int BP = SP - 1; // Base pointer - Points to base of current activation record (AR)
 	struct instruction IR; // Instruction Register
 	IR.op = 0;
  	IR.r = 0;
  	IR.l = 0;
 	IR.m = 0;


 	int stack[MAX_STACK_HEIGHT] = {0};
 	int RF[7] = {0}; // Register File


 	printf("-= Created by: Kensal J Ramos =-");
 	fetch(&PC, &SP, &BP, &IR, stack, RF);
 	return 0;
 }

/*
 *
 * Function Name: fetch()
 * Parameters: None	
 * Return Value: None
 *
 * Description: In the fetch cycle, an instruction is fetched from the “text” segment and 
 * 				placed in the IR register (IR <- text[PC]) and the program counter (PC) is 
 *				incremented by 1 to point to the next instruction to be executed (PC <- PC + 1). 
 *
 */
 void fetch(int *PC, int *SP, int *BP, struct instruction *IR, int *stack, int *RF) {

 	// Open input txt file
 	FILE *fp, *ofp;
 	struct instruction instArr[MAX_CODE_LENGTH] = {0};

 	// Open input.txt file
 	fp = fopen("input.txt", "r");

 	if (fp == NULL) {
 		fprintf(stderr, "Input.txt is null!");
 		exit(1);
 	}

 	// Create output.txt file
 	ofp = fopen("output.txt", "w");

 	if (ofp == NULL) {
 		fprintf(stderr, "Can't open output.txt!");
 		exit(1);
 	}

 	// Traverse input.txt file and save information in correct format to instruction array (instArr)
 	while (!feof(fp)) {
 		fscanf(fp, "%d %d %d %d", &instArr[*PC].op, &instArr[*PC].r, &instArr[*PC].l, &instArr[*PC].m);
 		//printf("\nInstruction has: %d %d %d %d", instArr[*PC].op, instArr[*PC].r, instArr[*PC].l, instArr[*PC].m);
 		(*PC)++;

 	}

 	// Commence execute cycle
 	execute(ofp, instArr, PC, SP, BP, IR, stack, RF);
 	
 	fclose(fp);
 	fclose(ofp);
 }

/*
 *
 * Function Name: execute()
 * Parameters: struct instruction inst - The current instruction
 * Return Value: None
 *
 * Description: In the Execute cycle, the instruction placed in the instruction register (IR), is executed by the VM-CPU. 
 *				The OP component that is stored in the IR register (IR.OP) indicates the operation 
 *				to be executed. For example, if IR.OP is the instruction ADD (IR.OP = 11), 
 *				then R, L, M components of the instruction in IR are used as registers numbers to 
 *				execute the instruction ADD R L M (RF [R] <- RF [L] + RF [M]) 
 *
 *
 */
 void execute(FILE *ofp, struct instruction *instArr, int *PC, int *SP, int *BP, struct instruction *IR, int *stack, int *RF) {

	printf("\nLine\tOP\tR\tL\tM\n");

	// Print interpreted assembly language
	for (int i = 0; i < (*PC); i++) {

		(*IR) = instArr[i];
	 	// Determine the operation based on opCode
	 	// Keep in mind stack grows top down. So first index is stack[999]
	 	switch (IR->op) {

			case 1: // LIT
				printf("%d\t%s\t%d\t%d\t%d\n", i, "lit", IR->r, IR->l, IR->m);
				break;
			case 2: // RTN
				printf("%d\t%s\t%d\t%d\t%d\n", i, "rnt", IR->r, IR->l, IR->m);

				break;
			case 3: // LOD
				printf("%d\t%s\t%d\t%d\t%d\n", i, "lod", IR->r, IR->l, IR->m);
				break;
			case 4: // STO
				printf("%d\t%s\t%d\t%d\t%d\n", i, "sto", IR->r, IR->l, IR->m);
				break;
			case 5: // CAL
				printf("%d\t%s\t%d\t%d\t%d\n", i, "cal", IR->r, IR->l, IR->m);
				break;
			case 6: // INC
				printf("%d\t%s\t%d\t%d\t%d\n", i, "inc", IR->r, IR->l, IR->m);
				break;
			case 7: // JMP
				printf("%d\t%s\t%d\t%d\t%d\n", i, "jmp", IR->r, IR->l, IR->m);
				break;
			case 8: // JPC
				printf("%d\t%s\t%d\t%d\t%d\n", i, "jpc", IR->r, IR->l, IR->m);
				break;
			case 9: // SYS
				printf("%d\t%s\t%d\t%d\t%d\n", i, "sys", IR->r, IR->l, IR->m);
				break;
			case 10: // NEG
				printf("%d\t%s\t%d\t%d\t%d\n", i, "neg", IR->r, IR->l, IR->m);
				break;
			case 11: // ADD
				printf("%d\t%s\t%d\t%d\t%d\n", i, "add", IR->r, IR->l, IR->m);
				break;
			case 12: // SUB
				printf("%d\t%s\t%d\t%d\t%d\n", i, "sub", IR->r, IR->l, IR->m);
				break;
			case 13: // MUL
				printf("%d\t%s\t%d\t%d\t%d\n", i, "mul", IR->r, IR->l, IR->m);
				break;
			case 14: // DIV
				printf("%d\t%s\t%d\t%d\t%d\n", i, "div", IR->r, IR->l, IR->m);
				break;
			case 15: // ODD
				printf("%d\t%s\t%d\t%d\t%d\n", i, "odd", IR->r, IR->l, IR->m);
				break;
			case 16: // MOD
				printf("%d\t%s\t%d\t%d\t%d\n", i, "mod", IR->r, IR->l, IR->m);
				break;
			case 17: // EQL
				printf("%d\t%s\t%d\t%d\t%d\n", i, "eql", IR->r, IR->l, IR->m);
				break;
			case 18: // NEQ
				printf("%d\t%s\t%d\t%d\t%d\n", i, "neq", IR->r, IR->l, IR->m);
				break;
			case 19: // LSS
				printf("%d\t%s\t%d\t%d\t%d\n", i, "lss", IR->r, IR->l, IR->m);
				break;
			case 20: // LEQ
				printf("%d\t%s\t%d\t%d\t%d\n", i, "leq", IR->r, IR->l, IR->m);
				break;
			case 21: // GTR
				printf("%d\t%s\t%d\t%d\t%d\n", i, "gtr", IR->r, IR->l, IR->m);
				break;
			case 22: // GEQ
				printf("%d\t%s\t%d\t%d\t%d\n", i, "geq", IR->r, IR->l, IR->m);
				break;

	 	}
 	}

 	// Print initial values
 	printf("\n\t\t\top\tbp\tsp");
 	printf("\nInitial values\t\t%d\t%d\t%d", 0, (*BP), (*SP));

 	printf("\nRegisters: ");
 	for (int i = 0; i < 7; i++) 
 		printf("%d ", RF[i]);

 	printf("\nStack: ");
 	for (int i = 1; i <= 40; i++)
 		printf("%d ", stack[MAX_STACK_HEIGHT - i]);

 	printf("\n\n\t\t\top\tbp\tsp");


 	int halt = 1; // Halt will be used for 'sys' OpCode
	int baseVal = -1; // This will be used for separating activation records

 	// Actual calculations
	for (int programCounter = 0; halt == 1; programCounter++) {

		(*IR) = instArr[programCounter];
	 	// Determine the operation based on opCode
	 	// Keep in mind stack grows top down. So first index is stack[999]
	 	switch (IR->op) {

	 		// Save m in instruction register to register file at index r
			case 1: // LIT
				RF[IR->r] = IR->m; // RF[r] <- m
				printf("\n\n%d lit %d %d %d", programCounter, IR->r, IR->l, IR->m);
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/* 
			 * Return from subroutine
			 * In other words, update stack pointer to before base pointer
			 * Base pointer will become value 2 indexes before new stack pointer value
			 * Update programCounter (PC)
			 */
			case 2: // RTN
				printf("\n\n%d rtn %d %d %d", programCounter, IR->r, IR->l, IR->m);
				(*SP) = ((*BP) + 1);
				(*BP) = stack[(*SP) - 2];
				programCounter = (stack[(*SP) - 3]) - 1; // -1 because the for loop will increment right after 
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Load value into selected register at stack location 
			 * IR->r = Register index
			 * IR->l = Lexographical level
			 * IR->m = Offset
			 */
			case 3: // LOD
				printf("\n\n%d lod %d %d %d", programCounter, IR->r, IR->l, IR->m);
				printf("LOD at index %d", IR->r);
				RF[IR->r] = stack[(base(IR->l, (*BP), stack)) - IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Store value from selected register into stack location
			 * IR->r = Register index
			 * IR->l = Lexographical level
			 * IR->m = Offset
			 */
			case 4: // STO
				printf("\n\n%d sto %d %d %d", programCounter, IR->r, IR->l, IR->m);
				stack[base(IR->l, (*BP), stack) - IR->m] = RF[IR->r];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Call procedure at index M
			 * IR->r = N/A
			 * IR->l = Lexographical level
			 * IR->m = new PC value (PC - program counter)
			 */
			case 5: // CAL
				printf("\n\n%d cal %d %d %d", programCounter, IR->r, IR->l, IR->m);
				stack[(*SP) - 1] = base(IR->l, (*BP), stack);
				baseVal = base(IR->l, (*BP), stack);
				stack[(*SP) - 2] = (*BP);
				stack[(*SP) - 3] = programCounter + 1; 
				(*BP) = (*SP) - 1;
				programCounter = IR->m - 1;
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Allocate M memory words
			 * IR->r = N/A
			 * IR->l = N/A
			 * IR->M = Number of words to allocate and value of PC
			 */
			case 6: // INC
				printf("\n\n%d inc %d %d %d", programCounter, IR->r, IR->l, IR->m);
				(*SP) = (*SP) - IR->m;
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Jump to instruction M
			 * IR->r = N/A
			 * IR->l = N/A
			 * IR->M = Instruction to jump to and value of PC
			 */
			case 7: // JMP
				printf("\n\n%d jmp %d %d %d", programCounter, IR->r, IR->l, IR->m);
				programCounter = IR->m - 1;
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Jump to instruction M if R is 0
			 * IR->r = 0 - Procceed
			 * IR->l = N/A
			 * IR->M = Instruction to jump to and value of PC
			 */
			case 8: // JPC
				printf("\n\n%d jpc %d %d %d", programCounter, IR->r, IR->l, IR->m);
				if (RF[IR->r] == 0)
					programCounter = IR->m - 1;
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			/*
			 * Based on the value of m, does one of three things, explained further down.
			 * IR->r = Index of register for m = 0 ^ m = 1
			 * IR->l = N/A
			 * IR->M = 	0 - Write a register to the screen
			 *			1 - Read in input from the user and store it in a register
			 *			2 - End program
			 */
			case 9: // SYS
				printf("\n\n%d sys %d %d %d", programCounter, IR->r, IR->l, IR->m);
				if (IR->m == 1) 
					printf("\nRegister at %d = %d", IR->r, RF[IR->r]);
				else if (IR->m == 2) {
					scanf("%d", &RF[IR->r]);
					printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				}
				else if (IR->m == 3) {
					halt = 0; // End program
					printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				} 
				break;
			// Change sign of value in register file (RF)
			case 10: // NEG
				printf("\n\n%d neg %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = -RF[IR->r];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// Add values at index l and m, store into index r
			case 11: // ADD
				printf("\n\n%d add %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] + RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// Subtract values at index l and m, store into index r
			case 12: // SUB
				printf("\n\n%d sub %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] - RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// Multiply values at index l and m, store into index r
			case 13: // MUL
				printf("\n\n%d mul %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] * RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// Divide values at index l and m, store into index r
			case 14: // DIV
				printf("\n\n%d div %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] / RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If value at index r is odd, set value at register r to 1
			case 15: // ODD
				printf("\n\n%d odd %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->r] % 2;
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// Mod values at index l and m, store into index r
			case 16: // MOD
				printf("\n\n%d mod %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] % RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at indexes l and m are equal, value at r is 1
			case 17: // EQL
				printf("\n\n%d eql %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] == RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at indexes l and m are not equal, value at r is 1
			case 18: // NEQ
				printf("\n\n%d neq %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] != RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at index l is less than m, value at r is 1
			case 19: // LSS
				printf("\n\n%d lss %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] < RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at index l is less than or equal to m, value at r is 1
			case 20: // LEQ
				printf("\n\n%d leq %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] <= RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at index l is greater than m, value at r is 1
			case 21: // GTR
				printf("\n\n%d gtr %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] > RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;
			// If values at index l is greater than or equal to m, value at r is 1
			case 22: // GEQ
				printf("\n\n%d geq %d %d %d", programCounter, IR->r, IR->l, IR->m);
				RF[IR->r] = RF[IR->l] >= RF[IR->m];
				printf("\t\t%d\t%d\t%d", programCounter + 1, (*BP), (*SP));
				break;

	 	}

	 	// Print registers and stack
 		printf("\nRegisters: ");
 		for (int j = 0; j < 7; j++) 
 			printf("%d ", RF[j]);

 		int j = 0;

 		// Print the stack until we hit a value equal to our baseVal
 		// If this occurs, we have entered a new Activation Record (AR)
 		printf("\nStack: ");
 		for (; j < MAX_STACK_HEIGHT - (*SP); j++) {

 			if (stack[MAX_STACK_HEIGHT - (j + 1)] == baseVal) {
 				printf("| ");
 				break;
 			}
 			printf("%d ", stack[MAX_STACK_HEIGHT - (j + 1)]);
 		}

 		// Continue with stack printing
 		for (; j < MAX_STACK_HEIGHT - (*SP); j++) {
 			printf("%d ", stack[MAX_STACK_HEIGHT - (j + 1)]);
 		}
 	}



 	fflush(ofp);

 }
 
// l stand for L in the instruction format
int base(int l, int base, int *stack) {  
  int b1; //find base L levels up
  b1 = base; 
  while (l > 0) {
    b1 = stack[b1];
    l--;
  }
  return b1;
}
