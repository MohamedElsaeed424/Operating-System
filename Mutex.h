#ifndef OPERATING_SYSTEM_MUTEX_H
#define OPERATING_SYSTEM_MUTEX_H
#include "Process.h"

typedef struct {
    Process* queue[3];
    int front;
    int rear;
    int capacity;
}LockQueue;

typedef struct {
    enum {zero,one} value;
    LockQueue queue;
    int ownerID;
}MUTEX;

enum state {Failed = -1, Good = 0, Blocked = 1};

void init_mutex(MUTEX *mutex) {
    mutex->value = zero;  // Assuming 'zero' means unlocked
    mutex->queue.front = 0;
    mutex->queue.rear = -1;
    mutex->queue.capacity = 3;
    mutex->ownerID = -1;  // No owner initially
}

int isQueueEmpty(LockQueue* queues) {
    return queues->rear == -1;
}
int isQueueFull(LockQueue* queues) {
    return queues->rear+1 == queues->capacity;
}
void enqueue(LockQueue *queue, Process * process) {
//    if (queue->rear == 2) {
//        queue->rear = -1;
//    }
//    if (queue->front == -1)
//        queue->front = 0;
//    queue->queue[++queue->rear] = process;
    if(isQueueFull(queue)){
        printf("Queue is Full\n");
        return;
    }
    queue->queue[++queue->rear] = process;

}

Process* dequeue(LockQueue *queue) {
//    if (queue->front == -1) {
//        printf("Queue is empty\n");
//        return NULL;
//    }
//    if (queue->front == 3) {
//        queue->front = -1;
//    }
//    Process *process = &queue->queue[queue->front];
//    if (queue->front == queue->rear)
//        queue->front = queue->rear = -1;
//    else
//        queue->front++;
//    return process;
    int highest = 5;
    int highi = 0;
    for(int i = 0; i < queue->rear; i++){
        if(queue->queue[i]->pcb->currentPriority < highest){
            highest = queue->queue[i]->pcb->currentPriority;
            highi = i;
        }
    }
    Process *process = queue->queue[highi];
    queue->rear--;
    for(int i = highi; i < queue->rear; i++){
        queue->queue[i] = queue->queue[i+1];
    }
    return process;
}
void semWait(MUTEX *m , Process * p ) {
    if (m->value == one) {
        m->ownerID = p->pcb->processID;
        m->value = zero;
    } else {
        enqueue(&m->queue,p);
    }
}
void semSignal(MUTEX *m , Process * p) {
    if(m->ownerID == p->pcb->processID){
        if (isQueueEmpty(&m->queue))
            m->value = one;
        else {
            Process * proc = dequeue(&m->queue);
            m->ownerID = proc->pcb->processID;
        }
    }
}


#endif //OPERATING_SYSTEM_MUTEX_H
