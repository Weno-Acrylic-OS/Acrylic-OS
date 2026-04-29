// src/kernel/task.c
#include <task.h>
#include <stddef.h> // For NULL
#include <libc.h>

static Task tasks[10]; // Simple array for tasks
static int next_task_id = 0;

void task_init() {
    // Initialize task management system
    for (int i = 0; i < 10; ++i) {
        tasks[i].id = -1; // Mark as unused
    }
    next_task_id = 0;
}

Task* task_create(void (*entry)(void*), void* arg, int priority, uint32_t stack_size) {
    if (next_task_id >= 10) {
        return NULL; // No more task slots
    }

    Task* new_task = &tasks[next_task_id++];
    new_task->id = next_task_id;
    new_task->entry = entry;
    new_task->arg = arg;
    new_task->priority = priority;
    new_task->state = TASK_READY;
    new_task->stack_size = stack_size;

    new_task->stack_base = (uint32_t*)malloc(stack_size);
    if (!new_task->stack_base) {
        return NULL; // Stack allocation failed
    }

    // Initialize the stack for the new task
    uint32_t* stack_top = new_task->stack_base + stack_size / sizeof(uint32_t);

    // ARM Cortex-M stack frame initialization
    *(--stack_top) = 0x01000000; // xPSR (Thumb bit set)
    *(--stack_top) = (uint32_t)(void*)entry; // PC
    *(--stack_top) = 0; // LR
    *(--stack_top) = 0; // R12
    *(--stack_top) = 0; // R3
    *(--stack_top) = 0; // R2
    *(--stack_top) = 0; // R1
    *(--stack_top) = (uint32_t)arg; // R0

    // The remaining registers (R4-R11) are not strictly necessary to initialize,
    // but we can zero them out for consistency.
    for (int i = 0; i < 8; ++i) {
        *(--stack_top) = 0;
    }

    new_task->sp = stack_top;

    return new_task;
}

void task_yield() {
    // For a preemptive scheduler, this is not strictly necessary,
    // but can be implemented to voluntarily give up the CPU.
    // It would trigger a context switch.
}
