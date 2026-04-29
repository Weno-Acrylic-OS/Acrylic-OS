// include/kernel/task.h
#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_TERMINATED
} TaskState;

typedef struct Task {
    // Task control block members
    uint32_t* sp; // Stack pointer
    int id;
    void (*entry)(void*);
    void* arg;
    int priority;
    TaskState state;
    uint32_t* stack_base;
    uint32_t stack_size;
} Task;

void task_init();
Task* task_create(void (*entry)(void*), void* arg, int priority, uint32_t stack_size);
void task_yield(); // To voluntarily give up CPU

#endif // TASK_H
