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

extern void printStack(Stack stack);
extern void print(Thread thread);
extern u_int pop(Thread* thread);
extern void push(Thread* thread, u_int opCode);
extern void init(Thread* thread);
extern void addThread(Stack* stack, Thread* thread);
extern void initStack(Stack* stack);
extern void processStack(const Stack* stack);
