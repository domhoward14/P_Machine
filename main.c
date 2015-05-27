#include<stdlib.h>
#include<stdio.h>
#include"compiler.h"
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define NEXT_INSTRUCTION 3

struct Instruction
{
    int op;
    int l;
    int m;

};

int sp = 0;
int bp = 1;
int pc = 0;
int stack [MAX_STACK_HEIGHT ];
int code [MAX_CODE_LENGTH ];
FILE *codeFile;
struct Instruction ir;

int main()
{
    load();
    while(!isDone())
    {
        fetch();
        execute();
    }
    return 0;
}


void load()
{
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    takingInput();
    fclose(codeFile);
}

int takingInput()
{
    codeFile = fopen("mcode.txt", "r");
    if(!codeFile)
    {
        printf("Error in opening the file.");
        exit(0);

    }
    int i = 0;
    int x, y;
    while ((x = fgetc(codeFile)) != EOF)
    {
        if(i % 3 == 0)
            printf("\n");
        if((int)x == 32 || (int)x == 10)
            continue;
        x -= 48;
        if(x == 1)
        if((y = (int)fgetc(codeFile)) != 32  )
            x = 10 + (y - 48);
        code[i] = x;
//      printf("%d",code[i]);
        i ++;
    }
}

void fetch ()
{
    getInstruction();
    pc += NEXT_INSTRUCTION;
}

void getInstruction()
{
    ir.op = code[pc];
    ir.l = code[pc + 1];
    ir.m =  code[pc + 2];
    printf("%d,",code[pc]);
    printf("%d,",code[pc + 1]);
    printf("%d \n",code[pc + 2]);
}

int isDone ()
{
    if(code[pc] == 9 && code[pc + 2] == 3)
        return 1;
    return 0;
}

void execute()
{
    printf("This is when the code will be executed.\n");
}