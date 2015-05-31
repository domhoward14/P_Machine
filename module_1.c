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

int lexiLevel = 0;
int partition = 0;
int codeSize = 0;
int buffer = 0;
int sp = 0;
int bp = 1;
int pc = 0;
int stack [MAX_STACK_HEIGHT ];
int code [MAX_CODE_LENGTH ];
FILE *codeFile;
FILE *output;
char *opString;
struct Instruction ir;

int main()
{
    output = fopen("output.txt", "w");
    initArray();
    load();
    //testFunction();
    format2();
    while(1==1)
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
    format();
    fclose(codeFile);
}

void format ()
{
    fprintf(output,"Line\t");
    fprintf(output,"OP\t");
    fprintf(output,"L\t");
    fprintf(output,"M\n");
    for(int i = 0; i < codeSize; i ++)
    {
        if(i % 3 == 0)
        {
            fprintf(output, "%d\t%s\t%d\t%d\n", i/3, getOpString(code[i]), code[i + 1], code[i + 2]);
        }
    }
}

void getCode()
{
    int i = 0;
    codeFile = fopen("mcode.txt", "r");
    while(fscanf(codeFile, "%d", &buffer)!=EOF)
    {
        code[i]=buffer;
        codeSize++;
        //testing purposes
        //printf("%d", code[i]);
        i++;
        if(i % 3 == 0)
            printf("\n");
    }
}

void fetch ()
{
    getInstruction();
    pc ++;
    //printf("After it is %d \n",pc);
}

void format2()
{
    fprintf(output, "\t\t\t\tpc\t\tbp\tsp\tstack\n");
    fprintf(output, "Initial Values\t\t\t0\t\t1\t0\n");

}
void getInstruction()
{
    //For testing purposes
    //printf("during the fetch pc is %d \n",pc);
    //print out the instruction number, opcode string, l, and m.
    ir.op = code[pc * NEXT_INSTRUCTION];
    ir.l = code[pc * NEXT_INSTRUCTION + 1];
    ir.m =  code[pc * NEXT_INSTRUCTION + 2];
    opString = getOpString(ir.op);
    fprintf(output, "%d\t%s\t%d\t%d", pc, opString, ir.l, ir.m );
}

char *getOpString(int op)
{
    switch (op)
        {
        //lit
        case 1:
            return "lit";
            break;
        // opr will require another sub-function to decide
        // which operation to run
        case 2:
            return "opr";
            break;
        //lod
        case 3:
            return "lod";
            break;
        //sto
        case 4:
            return "sto";
            break;
        //cal
        case 5:
            return "cal";
            break;
        //inc
        case 6:
            return "inc";
            break;
        //jmp
        case 7:
            return "jmp";
            break;
        //jpc
        case 8:
            return "jpc";
            break;
        //sio
        //this will require another sub function to decide
        //which i/o to run
        case 9:
            return "sio";
            break;

        default:
            fprintf(output, "OP code input was invalid. ");
            sio3();
        }

}

int isDone ()
{
    if(code[pc * NEXT_INSTRUCTION] == 9 && code[pc * NEXT_INSTRUCTION + 2] == 3)
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
            fprintf(output, "OP code input was invalid. ");
            sio3();
        }
        //print pc, bp, sp
        //use loop to print stack
        fprintf(output, "\t%d\t\t%d\t%d\t", pc, bp, sp);
        stackPrint();
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
    printf(" the very last number should be 3? = %d", code[50]);
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
    pc = pro_Location;
}

void inc (int space)
{
    sp += space;
    lexiLevel ++;
}

void jmp (int jumpSpot)
{
    pc = jumpSpot;
}

void jpc (int jumpSpot)
{
    if (stack[sp] == 0)
        pc = jumpSpot;
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
    fprintf(output, "\t%d\t\t%d\t%d\t", pc, bp, sp);
    stackPrint();
    exit(0);
}

void stackPrint()
{
    if(lexiLevel == 1)
    {
        for(int i = 1; i <= sp; i ++)
            fprintf(output, "%d ", stack[i]);
        fprintf(output, "\n");

    }
    else if(lexiLevel == 2)
    {
        for(int i = 1; i <= sp; i ++)
        {
            if(i == bp)
                fprintf(output, "| ");
            fprintf(output, "%d ", stack[i]);

        }
        fprintf(output, "\n");
    }
    else
    {
       int base = getBase(1, bp);
       for(int i = 1; i <= sp; i ++)
        {
            if(i == bp || base)
                fprintf(output, "| ");
            fprintf(output, "%d ", stack[i]);

        }
        fprintf(output, "\n");

    }


}

void ret ()
{
    sp = bp - 1;
    pc = stack[sp + 4];
    bp = stack[sp + 3];
    lexiLevel --;
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
            fprintf(output, "OP code input was invalid. ");
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
            fprintf(output, "OP code input was invalid. ");
            sio3();
    }

}



