/* * * * * * * * * * * * * * * * * * * * * * 
 * COP 3402 - Project #1 VM stack Machine  *
 * Group # 11                              *
 * David								   *
 * Nikole                                  *
 * Nicolas                                 *
 * Veronica                                *
* * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

int base(int *stack, int BP, int L);
void print_instruction(int PC, instruction IR);
void print_stack(int PC, int BP, int SP, int *stack);


void execute(int trace_flag, instruction *code)
{
	int* stack;
	int *PC , BP = 0,
	    halt = 0, SP = -1;
	
	instruction IR;
	*PC = 0;

	
	
	if (trace_flag)
	{
		stack = calloc(ARRAY_SIZE, sizeof(int));

		while(!trace_flag) 
		{	
			switch(IR.op)
			{
				case 1:
					SP = SP + 1;
					stack[SP] = IR.m;
					break;
					
				case 2:

					switch(IR.m)
					{
						case 1:
							SP = SP - 1;
							stack[SP] = stack[SP] + stack[SP + 1];
							break;
							
						case 2:
							SP = SP - 1;
							stack[SP] = stack[SP] - stack[SP + 1];
							break;
						
						case 3:
							
							SP = SP - 1;
							stack[SP] = stack[SP] * stack[SP + 1];
							break;
						
						case 4:
							SP = SP - 1;
							stack[SP] = stack[SP] / stack[SP + 1];
							break;
						
						case 5:
							SP = SP - 1;
							stack[SP] = stack[SP] == stack[SP + 1];
							break;
						
						case 6:
							SP = SP - 1;
							stack[SP] = stack[SP] != stack[SP + 1];
							break;
						
						case 7:
							SP = SP - 1;
							stack[SP] = stack[SP] < stack[SP + 1];
							break;
							
						case 8:
							SP = SP - 1;
							stack[SP] = stack[SP] <= stack[SP + 1];
							break;
						
						case 9:
							SP = SP - 1;
							stack[SP] = stack[SP] > stack[SP + 1];
							break;

						case 10:
							SP = SP - 1;
							stack[SP] = stack[SP] >= stack[SP + 1];
							break;
					}

				case 3:
					SP = SP + 1;
					stack[SP] = stack[base(stack, BP, IR.l) + IR.m];
					break;
				case 4:
					stack[base(stack, BP, IR.l) + IR.m] = stack[SP];
					SP = SP - 1;
					break;
				case 5:
					stack[SP + 1] = base(stack, BP, IR.l); //static link (SL)
					stack[SP + 2] = BP; //dyanmic link (DL)
					stack[SP + 3] = PC; //return address (RA)
					BP = SP + 1;
					PC = IR.m;
					break;	
				case 6:
					SP = BP - 1;
					BP = stack[SP + 2]; //dynamic link (DL)
					PC = stack[SP + 3]; //return address (RA)
					break;
				case 7:
					SP = SP + IR.m;
					break;
				case 8:
					PC = IR.m;
				case 9:
					if(stack[SP] == 0)
					{
						PC = IR.m;
					}
					SP = SP - 1;
					break;
				case 10:
					switch(IR.m)
					{
						case 1:
							printf("\nOutput: %d", stack[SP]);
							printf("\n\t\t\t\t");
							scanf("%d", &stack[SP]);
							printf("\t\t\t\t");
							break;

						case 2:
							SP = SP + 1;
							printf("\nInput: ");
							scanf("%d", &stack[SP]);
							printf("\t\t\t\t");

						case 3:
							halt = 1;
					}


			}

		}
		printf("VM Exectution:\n");
		printf("\t\t\t\tPC\tBP\tSP\tstack\n");
		printf("Initial Values:\t\t\t0\t0\t-1\n");
	}
}

int base(int *stack, int BP, int L)
{
	while (L > 0)
	{
		BP = stack[BP];
		L--;
	}
	return BP;
}


void print_stack(int PC, int BP, int SP, int *stack)
{
	int i;
	printf("%d\t%d\t%d\t", PC, BP, SP);

	for (i = 0; i <= SP; i++)
		printf("%d ", stack[i]);
		
	printf("\n");
}

void print_instruction(int PC, instruction IR)
{
	char opname[4];
	
	switch (IR.op)
	{
		case LIT : strcpy(opname, "LIT"); break;
		case OPR :
			switch (IR.m)
			{
				case ADD : 
					strcpy(opname, "ADD"); 
					break;
				case SUB : 
					strcpy(opname, "SUB"); 
					break;
				case MUL :
					strcpy(opname, "MUL"); 
					break;
				case DIV : 
					strcpy(opname, "DIV");
					break;
				case EQL : 
					strcpy(opname, "EQL"); 
					break;
				case NEQ : 
					strcpy(opname, "NEQ"); 
					break;
				case LSS : 
					strcpy(opname, "LSS"); 
					break;
				case LEQ : 
					strcpy(opname, "LEQ"); 
					break;
				case GTR : 
					strcpy(opname, "GTR"); 
					break;
				case GEQ : 
					strcpy(opname, "GEQ"); 
					break;
				default : 
					strcpy(opname, "err"); 
					break;
			}
			break;

		case LOD : 
			strcpy(opname, "LOD"); 
			break;
		case STO : 
			strcpy(opname, "STO"); 
			break;
		case CAL : 
			strcpy(opname, "CAL"); 
			break;
		case RTN : 
			strcpy(opname, "RTN"); 
			break;
		case INC : 
			strcpy(opname, "INC"); 
			break;
		case JMP : 
			strcpy(opname, "JMP"); 
			break;
		case JPC : 
			strcpy(opname, "JPC"); 
			break;
		case SYS : 
			switch (IR.m)
			{
				case WRT : 
					strcpy(opname, "WRT"); 
					break;
				case RED : 
					strcpy(opname, "RED"); 
					break;
				case HLT : 
					strcpy(opname, "HLT"); 
					break;
				default : 
					strcpy(opname, "err"); 
					break;
			}
			break;
		default : strcpy(opname, "err"); break;
	}
	
	printf("%d\t%s\t%d\t%d\t", PC - 1, opname, IR.l, IR.m);
}