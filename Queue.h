#ifndef MSTWOOS_QUEUE_H
#define MSTWOOS_QUEUE_H
#include "PCB.h"
#include <stdlib.h>

typedef struct {
     PCB* process;
     Node* next;
}Node;

typedef struct  {
     Node* front;
     Node* rear;
}PCBQueue;

 typedef struct  {
     PCBQueue* queues[4];
} ReadyQueue;

void initRQ(ReadyQueue* readyQueue) {
    for (int i = 0; i < 4; i++) {
        readyQueue->queues[i].front = NULL;
        readyQueue->queues[i].rear = NULL;
    }
}


void enqueue(ReadyQueue* readyQueue,  PCB* process) {
    int priority = process->currentPriority - 1;
    Node* newP = (Node*)malloc(sizeof(Node));
    newP->process = process;
    if (readyQueue->queues[priority].rear == NULL) {
        readyQueue->queues[priority].front = readyQueue->queues[priority].rear = newP;
    } else {
        readyQueue->queues[priority].rear->next = newP;
        readyQueue->queues[priority].rear = newP;
    }
    process->processState = "READY"
    newP->next = NULL;
}


PCB* dequeue(ReadyQueue* readyQueue) {
    for (int i = 0; i < 4; i++) {
        if (readyQueue->queues[i].front != NULL) {
            Node* proc = readyQueue->queues[i].front;
            readyQueue->queues[i].front = readyQueue->queues[i].front->next;
            if (readyQueue->queues[i].front == NULL) readyQueue->queues[i].rear = NULL;
            if(proc->process->currentPriority < 4) proc->process->currentPriority++ ;
            proc->process->processState = "RUNNING";
            free(proc);
            return proc->process;
        }
    }
    return NULL;
}

#endif //MSTWOOS_QUEUE_H
