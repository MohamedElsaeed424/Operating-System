#ifndef MSTWOOS_PCB_H
#define MSTWOOS_PCB_H
#include <string.h>
#include <stdio.h>
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
    pcb->pc = 0;
    pcb->memoryLowerBoundary = memoryLowerBoundary;
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


#endif //MSTWOOS_PCB_H
