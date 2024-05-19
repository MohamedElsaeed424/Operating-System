#ifndef MSTWOOS_QUEUE_H
#define MSTWOOS_QUEUE_H
#include "Process.h"
#include <stdlib.h>
#define LEVELS 4
#define MAX_PROCESSES 3

// front and rear are indices
typedef struct {
    Process *queue[MAX_PROCESSES];
    int front;
    int rear;
    int capacity;
    int time_quantum;
} Queue;
int isQueueEmpty(Queue* queues) {
    return queues->rear == -1;
}
int isQueueFull(Queue* queues) {
    return queues->rear+1 == queues->capacity;
}

void enqueue(Queue *queue, Process * process) {
    if(isQueueFull(queue)){
        printf("Queue is Full\n");
        return;
    }
    queue->queue[++queue->rear] = process;
}
Process* dequeue(Queue *queue) {
    Process *process = queue->queue[0];
    queue->rear--;
    for(int i = 0; i < queue->rear; i++){
        queue->queue[i] = queue->queue[i+1];
    }
    return process;
}

Queue queues[LEVELS];
const int quanta[] = {1, 2, 4, 8};

void initQueue() {
    for (int i = 0; i < LEVELS; i++) {
        queues[i].front = 0;
        queues[i].rear = -1;
        queues[i].time_quantum = quanta[i];
    }
}


void enqueueML(int level, Process *process) {

    if (queues[level].rear == (MAX_PROCESSES - 1)){
        printf("Queue of level %d is full\n", level);
        return;
    }
    enqueue(&queues[level], process);
}

Process* dequeueML(int level) {
    if(isQueueEmpty(&queues[level]))
        return NULL;
    Process *process = dequeue(&queues[level]);

    strcpy(process->pcb->processState, "RUNNING");
    return process;
}

#endif //MSTWOOS_QUEUE_H
