
#ifndef OPERATING_SYSTEM_MUTEX_H
#define OPERATING_SYSTEM_MUTEX_H
typedef struct {
    int available;
    // queue
    int ownerId;
} Mutex;
#endif //OPERATING_SYSTEM_MUTEX_H
