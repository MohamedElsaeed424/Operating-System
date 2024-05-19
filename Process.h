#ifndef MSTWOOS_PCB_H
#define MSTWOOS_PCB_H
#include <string.h>
#include <stdio.h>
#include "Memory.h"
enum offset {ID, processState, priority, pc, lowerBoundary, upperBoundary};
typedef struct  {
    int processID;
    char processState[10];
    int currentPriority;
    int pc;
    int memoryLowerBoundary;
    int memoryUpperBoundary;
}PCB;

typedef struct {
    int arrival_time;   // input
    int remaining_time; // num of lines in the program
    PCB* pcb;
}Process;


void initPCB( PCB *pcb , int processID ,int memoryLowerBoundary, int memoryUpperBoundary) {
    pcb->processID = processID;
    strcpy(pcb->processState, "READY");
    pcb->currentPriority = 1;
    pcb->memoryLowerBoundary = memoryLowerBoundary;
    pcb->pc = memoryLowerBoundary+9;
    pcb->memoryUpperBoundary = memoryUpperBoundary;
}

void printPCB(PCB *pcb) {
    printf("Process ID: %d\n", pcb->processID);
    printf("Process State: %s\n", pcb->processState);
    printf("Current Priority: %d\n", pcb->currentPriority);
    printf("Program Counter: %d\n", pcb->pc);
    printf("Memory Lower Boundary: %d\n", pcb->memoryLowerBoundary);
    printf("Memory Upper Boundary: %d\n", pcb->memoryUpperBoundary);
}
void changeState(PCB* pcb, Memory* memory, char state[]){
    strcpy(pcb->processState, state);
    strcpy(memory->words[pcb->memoryLowerBoundary+lowerBoundary].value, state);
}
void incPriority(PCB* pcb, Memory* memory){
    if(pcb->currentPriority == 4)
        return;
    pcb->currentPriority++;
    char s[20];
    sprintf(s, "%d", pcb->currentPriority);
    strcpy(memory->words[pcb->memoryLowerBoundary + priority].value, s);
}
void incPC(PCB* pcb, Memory* memory){
    pcb->pc++;
    char s[20];
    sprintf(s, "%d", pcb->pc);
    strcpy(memory->words[pcb->memoryLowerBoundary + pc].value, s);
}


#endif //MSTWOOS_PCB_H
