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

enum state {Failed = -1, Success = 0, Blocked = 1};

void init_mutex(MUTEX *mutex) {
    mutex->value = one;  // Assuming 'zero' means unlocked. No 'one means unlocked you twat
    mutex->queue.front = 0;
    mutex->queue.rear = -1;
    mutex->queue.capacity = 3;
    mutex->ownerID = -1;  // No owner initially
}

int isLockQueueEmpty(LockQueue* queues) {
    return queues->rear == -1;
}
int isLockQueueFull(LockQueue* queues) {
    return queues->rear+1 == queues->capacity;
}
void enqueueLock(LockQueue *queue, Process * process) {
    if(isLockQueueFull(queue)){
        printf("Queue is Full\n");
        return;
    }
    queue->queue[++queue->rear] = process;

}

Process* dequeueLock(LockQueue *queue) {
    int highest = 5;
    int highi = 0;
    for(int i = 0; i < queue->rear; i++){
        if(queue->queue[i]->pcb->currentPriority < highest){
            highest = queue->queue[i]->pcb->currentPriority;
            highi = i;
        }
    }
    Process *process = queue->queue[highi];
    for(int i = highi; i < queue->rear; i++){
        queue->queue[i] = queue->queue[i+1];
    }
    queue->rear--;
    return process;
}
enum state semWait(MUTEX *m , Process * p ) {
    if (m->value == one) {
        m->ownerID = p->pcb->processID;
        m->value = zero;
        return Success;
    }
    enqueueLock(&m->queue,p);
    return Blocked;
}
Process *semSignal(MUTEX *m , Process* p) {
    if(m->ownerID == p->pcb->processID){
        if (isLockQueueEmpty(&m->queue)){
            m->value = one;
            return NULL;
        }
        else {
            Process * proc = dequeueLock(&m->queue);
            m->ownerID = proc->pcb->processID;
            return proc;
        }
    }
    return NULL;
}


#endif //OPERATING_SYSTEM_MUTEX_H
