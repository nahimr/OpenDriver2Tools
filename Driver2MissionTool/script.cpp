#include "script.hpp"

#include <cstdio>
#include <cstdlib>
#include "mission.hpp"

void initStack(Stack* stack)
{
    stack->nbThreads = 0;
    stack->nbOperations = 0;
    stack->operations = static_cast<u_int*>(malloc(sizeof(u_int) * 128));
    stack->threadOffsets = static_cast<u_int*>(malloc(sizeof(u_int) * 16));

    for (int i = 0; i < 128; i++)
    {
        stack->operations[i] = 0;
    }
}

void addThread(Stack* stack, Thread* thread)
{
    stack->threadOffsets[stack->nbThreads] = stack->nbOperations;
    stack->nbThreads++;
    for (unsigned int i = stack->nbOperations; i < (stack->nbOperations + thread->offset); i++)
    {
        stack->operations[i] = thread->operations[i - stack->nbOperations];
    }
    stack->nbOperations += thread->offset;
}

void init(Thread* thread)
{
    thread->operations = static_cast<Operation*>(malloc(sizeof(Operation) * 128));
    thread->offset = 0;

    for (u_int i = 0; i < 128; i++)
    {
        thread->operations[i] = NULL;
    }
}

void push(Thread* thread, const u_int opCode)
{
    thread->operations[thread->offset++] = opCode;
}

u_int pop(Thread* thread)
{
    const u_int popValue = thread->operations[thread->offset];
    thread->operations[thread->offset] = NULL;
    thread->offset--;

    return popValue;
}

void print(Thread thread)
{
    Operation* operations = thread.operations;
    for (int i = 0; i < thread.offset; i++)
    {
        const u_int addr = (operations + i) - operations;
        printf("(%u): %u\n", addr, operations[i]);
    }

}

void printStack(const Stack stack)
{
    for (int i = 0; i < stack.nbOperations; i++)
    {
        const u_int addr = (stack.operations + i) - stack.operations;
        printf("(%u): 0x%x\n", addr, stack.operations[i]);
    }
}

void processStack(const Stack* stack)
{
    for (int i = 0; i < stack->nbOperations; i++)
    {
        // Check if this is a thread call
        if (stack->operations[i] == CMD_StartThreadForPlayer || stack->operations[i] == CMD_StartThread2)
        {
            // If yes we want to replace the threadId by the offset where it's in the stack
            const u_int threadId = stack->operations[i - 1];
            stack->operations[i - 1] = stack->threadOffsets[threadId] - i - 1;
        }
    }
}