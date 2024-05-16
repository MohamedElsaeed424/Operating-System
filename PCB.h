#ifndef MSTWOOS_PCB_H
#define MSTWOOS_PCB_H
#include "string.h"
typedef struct  {
    int processID;
    char processState[10];
    int currentPriority;
    int programCounter;
    int memoryLowerBoundary;
    int memoryUpperBoundary;
}PCB;

void initPCB( PCB *pcb) {
    pcb->processID = processID;
    pcb->processState = "NEW";
    pcb->currentPriority = 1;
    pcb->programCounter = 0;
    pcb->memoryLowerBoundary = 0;
    pcb->memoryUpperBoundary = 0;
}

#endif //MSTWOOS_PCB_H
