#ifndef OPERATING_SYSTEM_MUTEX_H
#define OPERATING_SYSTEM_MUTEX_H
#include "Process.h"
typedef struct {
    Process* queue[3];
    int front;
    int rear;
}LockQueue;

typedef struct {
    enum {zero,one} value;
    LockQueue queue;
    int ownerID;
}MUTEX;

void init_mutex(MUTEX *mutex) {
    mutex->value = zero;  // Assuming 'zero' means unlocked
    mutex->queue.front = -1;
    mutex->queue.rear = -1;
    mutex->ownerID = -1;  // No owner initially
}
int isQueueEmpty(LockQueue queues) {
    return queues.front == -1;
}
void enqueue(LockQueue *queue, Process * process) {
    if (queue->rear == 2) {
        printf("Queue is full\n");
        return;
    }
    if (queue->front == -1)
        queue->front = 0;
    queue->queue[++queue->rear] = process;
}
Process* dequeue(LockQueue *queue) {
    if (queue->front == -1) {
        printf("Queue is empty\n");
        return NULL;
    }
    Process *process = &queue->queue[queue->front];
    if (queue->front == queue->rear)
        queue->front = queue->rear = -1;
    else
        queue->front++;
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
        if (isQueueEmpty(m->queue))
            m->value = one;
        else {
            Process * proc = dequeue(&m->queue);
            m->ownerID = proc->pcb->processID;
        }
    }
}


#endif //OPERATING_SYSTEM_MUTEX_H
