#ifndef MSTWOOS_QUEUE_H
#define MSTWOOS_QUEUE_H
#include "Process.h"
#include <stdlib.h>
#define LEVELS 4
#define MAX_PROCESSES 3

// front and rear are indices
typedef struct {
    Process queue[MAX_PROCESSES];
    int front;
    int rear;
    int time_quantum;
} Queue;

Queue queues[LEVELS];

void initQueue( int quanta[]) {
    for (int i = 0; i < LEVELS; i++) {
        queues[i].front = -1;
        queues[i].rear = -1;
        queues[i].time_quantum = quanta[i];
    }
}

void enqueue(int level, Process process) {
    if (queues[level].rear == (MAX_PROCESSES - 1)){
        printf("Queue of level %d is full\n", level);
        return;
    }
    if (queues[level].front == -1)
        queues[level].front = 0;
    queues[level].queue[++queues[level].rear] = process;  // increment rear and insert element
}

Process* dequeue(int level) {
    if (queues[level].front == -1)
        return NULL; // Queue is empty
    Process *process = &queues[level].queue[queues[level].front]; // get the element at front
    if (queues[level].front == queues[level].rear) // if there is only one element in queue
        queues[level].front = queues[level].rear = -1; // reset front and rear to -1
    else
        queues[level].front++;
    strcpy(process->pcb->processState, "RUNNING");
    return process;
}

int isQueueEmpty(int level) {
    return queues[level].front == -1;
}
void MLFQ(int quanta[]) {
    for (int level = 0; level < LEVELS; level++) {
        if (!isQueueEmpty(level)) {
            Process *p = dequeue(level);
            int exec_time = (p->remaining_time < quanta[level]) ? p->remaining_time : quanta[level]; // execute for time quantum or remaining time
            // -------------------------------------Execute the process-----------
            p->remaining_time -= exec_time; // update remaining time for the process
            if (p->remaining_time > 0) { // if process is not finished
                int next_level = (level == LEVELS - 1) ? level : level + 1;// if reached last level keep .if not move to next lower priority queue
                enqueue(next_level, *p);
            } else {
                process_executed++;
            }
            break;
        }
    }
}


#endif //MSTWOOS_QUEUE_H
