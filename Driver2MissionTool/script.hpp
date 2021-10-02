#pragma once

typedef unsigned int u_int;
typedef u_int Operation;

typedef struct Thread
{
    Operation* operations;
    u_int offset;
} Thread;

typedef Thread* Script;

typedef struct Stack
{
    u_int* operations; // Concatenation of all operations threads
    u_int* threadOffsets; // Address(index) where each thread start (16 max)
    u_int nbThreads; // >= 1 (max.16)
    u_int nbOperations; // Number of operations (sum of each nbOperations of all Thread)
} Stack;

void printStack(Stack stack);
void print(Thread thread);
u_int pop(Thread* thread);
void push(Thread* thread, u_int opCode);
void init(Thread* thread);
void addThread(Stack* stack, Thread* thread);
void initStack(Stack* stack);
void processStack(Stack* stack);