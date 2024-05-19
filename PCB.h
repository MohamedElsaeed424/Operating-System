#ifndef MSTWOOS_PCB_H
#define MSTWOOS_PCB_H
#include <string.h>
#include <stdio.h>
typedef struct  {
    int processID;
    char processState[10];
    int currentPriority;
    int programCounter;
    int memoryLowerBoundary;
    int memoryUpperBoundary;
}PCB;


void initPCB( PCB *pcb , int processID ,int memoryLowerBoundary, int memoryUpperBoundary) {
    pcb->processID = processID;
    strcpy(pcb->processState, "NEW");
    pcb->currentPriority = 1;
    pcb->programCounter = 0;
    pcb->memoryLowerBoundary = memoryLowerBoundary;
    pcb->memoryUpperBoundary = memoryUpperBoundary;
}

void printPCB(PCB *pcb) {
    printf("Process ID: %d\n", pcb->processID);
    printf("Process State: %s\n", pcb->processState);
    printf("Current Priority: %d\n", pcb->currentPriority);
    printf("Program Counter: %d\n", pcb->programCounter);
    printf("Memory Lower Boundary: %d\n", pcb->memoryLowerBoundary);
    printf("Memory Upper Boundary: %d\n", pcb->memoryUpperBoundary);
}


#endif //MSTWOOS_PCB_H
