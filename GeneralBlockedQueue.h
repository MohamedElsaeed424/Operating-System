#ifndef OPERATING_SYSTEM_GENERALBLOCKEDQUEUE_H
#define OPERATING_SYSTEM_GENERALBLOCKEDQUEUE_H
#define MAX_PROCESSES 3
#include "Process.h"

typedef struct {
    Process *queue[MAX_PROCESSES];
    int front;
    int rear;
    int capacity;
} BlockedQueue;

void init_BlockedQueue(BlockedQueue *blockedQueue) {
    blockedQueue->front = 0;
    blockedQueue->rear = -1;
    blockedQueue->capacity = MAX_PROCESSES;
}
int isBlockedEmpty(BlockedQueue* queues) {
    return queues->rear == -1;
}
int isBlockedFull(BlockedQueue* queues) {
    return queues->rear+1 == queues->capacity;
}

void printBlocked(BlockedQueue* queue){
    printf("General Blocked Queue: ");
    for(int i = 0; i <= queue->rear; i++){
        printf("%d ",queue->queue[i]->pcb->processID);
    }
    printf("\n");
}
void enqueueBlocked(BlockedQueue *queue, Process * process) {
    if(isBlockedFull(queue)){
        printf("Queue is Full\n");
        return;
    }
    queue->queue[++queue->rear] = process;
    printf("\n");
    printf("enqueue process %d\n", process->pcb->processID);
    printBlocked(queue);
    printf("\n");
}
void dequeueBlocked(BlockedQueue *queue, Process* unblocked) {
    if(isBlockedEmpty(queue)){
        printf("General Blocked queue is empty\n");
        return;
    }
    int removed = -1;
    for(int i = 0; i <= queue->rear; i++){
        if(queue->queue[i] == unblocked){
            // or check if processId's are equal, if something goes wrong
            removed = i;
        }
    }
    int id = queue->queue[removed]->pcb->processID;

    for(int i = removed; i < queue->rear; i++){
        queue->queue[i] = queue->queue[i+1];
    }
    queue->rear--;

    printf("\n");
    printf("dequeue process %d\n", id);
    printBlocked(queue);
    printf("\n");
}


#endif //OPERATING_SYSTEM_GENERALBLOCKEDQUEUE_H
