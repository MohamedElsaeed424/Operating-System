#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCB.h"
#include "fileController.h"
#include "Memory.h"
#include "Queue.h"
ReadyQueue* readyQueue;

void executeProcess() {
    int timeQuantum[4] = {1, 2, 4, 8}; // Time quanta for each priority level

    while (1) {
        PCB* process = dequeue(readyQueue);
        if (process == NULL) {
            printf("No more processes to execute.\n");
            break;
        }
        int priority = process->currentPriority - 1;
        int quantum = timeQuantum[priority];

        if (process->remainingTime <= quantum) {
            printf("Process %d executed for %d time units and completed.\n", process->pid, process->remainingTime);
            process->remainingTime = 0;
            strcpy(process->processState, "TERMINATED");
            free(process); // Free the process memory as it is completed
        } else {
            printf("Process %d executed for %d time units.\n", process->pid, quantum);
            process->remainingTime -= quantum;
            strcpy(process->processState, "WAITING");
            enqueue(readyQueue, process); // Re-enqueue the process for next round
        }
    }
}
int main() {
    ReadyQueue* readyQueue = (ReadyQueue*)malloc(sizeof(ReadyQueue));
    initRQ(readyQueue);

    // read processes from file and create PCB for each process


    executeProcess(readyQueue);

    // Free allocated memory for ReadyQueue
    for (int i = 0; i < 4; i++) {
        free(readyQueue->queues[i]);
    }
    free(readyQueue);

    return 0;
}