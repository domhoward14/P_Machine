#include<stdlib.h>
#include<stdio.h>
#include"module_1.h"
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

int codeSize = 0;
int buffer = 0;
int sp = 0;
int bp = 1;
int pc = 0;
int stack [MAX_STACK_HEIGHT ];
int code [MAX_CODE_LENGTH ];
FILE *codeFile;
struct Instruction ir;

int main()
{
    initArray();
    load();
   // testFunction();
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
    getCode();
    fclose(codeFile);
}

void getCode()
{
    int i = 0;
    codeFile = fopen("mcode.txt", "r");
    while(fscanf(codeFile, "%d", &buffer)!=EOF)
    {
        code[i]=buffer;
        codeSize++;
        printf("%d", code[i]);
        i++;
        if(i % 3 == 0)
            printf("\n");

    }
}

void fetch ()
{
    getInstruction();
    printf("%d,",code[pc]);
    printf("%d,",code[pc + 1]);
    printf("%d \n",code[pc + 2]);
    printf("the pc before next is %d \n",pc);
    pc += NEXT_INSTRUCTION;
    printf("After it is %d \n",pc);
}

void getInstruction()
{
    printf("during the fetch pc is %d \n",pc);
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
    switch ( ir.op)
        {
        //lit
        case 1:
            lit(ir.m);
            break;
        // opr will require another sub-function to decide
        // which operation to run
        case 2:
            opr();
            break;
        //lod
        case 3:
            lod(ir.l, ir.m);
            break;
        //sto
        case 4:
            sto(ir.l, ir.m);
            break;
        //cal
        case 5:
            cal(ir.l, ir.m);
            break;
        //inc
        case 6:
            inc(ir.m);
            break;
        //jmp
        case 7:
            jmp(ir.m);
            break;
        //jpc
        case 8:
            jpc(ir.m);
            break;
        //sio
        //this will require another sub function to decide
        //which i/o to run
        case 9:
            sio();
            break;

        default:
            printf("OP code input was invalid. ");
            sio3();
        }
}
//initializes the stack and code arrays.
void initArray()
{
    for(int i = 0; i < MAX_STACK_HEIGHT; i ++)
        stack[0] = 0;
    for(int i = 0; i < MAX_CODE_LENGTH; i ++)
        code[0] = 0;

}
//iterates through the dynamic link values to return the desired AR.
int getBase(int level, int base)
{
    for(int i = level; i > 0; i --)
    {
        base = stack[base + 2];
    }
    return base;
}

//this is simply a push function
void lit (int pushValue)
{
    sp ++;
    stack[sp] = pushValue;
}

void testFunction()
{
    printf("%d", 3 == 4);
    sio3();
}

//lod function will do 3 things
//get the base value L levels down
//grab the number at offset M
//push that number
void lod (int levels, int offset)
{
    int base = getBase(levels, bp);
    int value = stack[base + offset];
    lit(value);
}

//this function is simply a store function
void sto (int levels, int offset)
{
    int base = getBase(levels, bp);
    stack [base + offset] = stack[sp];
    sp --;
}

void cal (int levels, int pro_Location)
{
    stack[sp + 1] = 0;
    stack[sp + 2] = getBase(levels, bp);
    stack[sp + 3] = bp;
    stack[sp + 4] = pc;
    bp = sp + 1;
    pc = pro_Location * NEXT_INSTRUCTION;
}

void inc (int space)
{
    sp += space;
}

void jmp (int jumpSpot)
{
    pc = NEXT_INSTRUCTION * jumpSpot;
}

void jpc (int jumpSpot)
{
    if (stack[sp] == 0)
        pc = NEXT_INSTRUCTION * jumpSpot;
    sp --;
}

//this is the first standard i/o function which will pop and print
void sio1()
{
    printf("%d" ,stack[sp]);
    sp --;
}

//the second i/o function will take in input and push it to the top of the stack
void sio2()
{
    int input;
    scanf("%d" ,&input);
    lit(input);
}

//Third standard i/o halts the system
void sio3()
{
    exit(0);
}

void ret ()
{
    sp = bp - 1;
    pc = stack[sp + 4];
    bp = stack[sp + 3];
}

void neg ()
{
    stack[sp] = -stack[sp];
}

void add ()
{
    sp --;
    stack[sp] = stack[sp] + stack[sp + 1];
}

void sub ()
{
    sp --;
    stack[sp] = stack[sp] - stack[sp + 1];
}

void mul ()
{
    sp --;
    stack[sp] = stack[sp] - stack[sp + 1];
}

void divid()
{
    sp --;
    stack[sp] = stack[sp] / stack[sp + 1];
}

void mod ()
{
    sp --;
    stack[sp] = stack[sp] % stack[sp + 1];
}

void eql ()
{
    sp --;
    stack[sp] = stack[sp] == stack[sp + 1];
}

void neq ()
{
    sp --;
    stack[sp] = stack[sp] != stack[sp + 1];
}

void lss ()
{
    sp --;
    stack[sp] = stack[sp] < stack[sp + 1];
}

void leq ()
{
    sp --;
    stack[sp] = stack[sp] <= stack[sp + 1];
}

void gtr ()
{
    sp --;
    stack[sp] = stack[sp] > stack[sp + 1];
}

void geq ()
{
    sp --;
    stack[sp] = stack[sp] >= stack[sp + 1];
}

void odd()
{
    stack[sp] = stack[sp] % 2;
}

void opr()
{
    switch ( ir.m)
    {
        //rtn
        case 0:
            ret();
            break;
        //neg
        case 1:
            neg();
            break;
       //add
        case 2:
            add();
            break;
       //sub
        case 3:
            sub();
            break;
        //mul
        case 4:
            mul();
            break;
        //div
        case 5:
            divid();
            break;
        //odd
        case 6:
            odd();
            break;
        //mod
        case 7:
            mod();
            break;
        //eql
        case 8:
            eql();
            break;
        //neq
        case 9:
            neq();
            break;
        //lss
        case 10:
            lss();
            break;
        //leq
        case 11:
            leq();
            break;
        //gtr
        case 12:
            gtr();
            break;
        //geq
        case 13:
            geq();
            break;

        default:
            printf("OP code input was invalid. ");
            sio3();
    }
}

void sio()
{
    switch ( ir.m)
    {
        //output to the screen
        case 1:
            sio1();
            break;

        //take input
        case 2:
            sio2();
            break;

        //halt
        case 3:
            sio3();
            break;

        default:
            printf("OP code input was invalid. ");
            sio3();
    }

}

