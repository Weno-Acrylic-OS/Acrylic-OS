// src/kernel/scheduler.c
#include <scheduler.h>
#include <task.h>
#include <stddef.h> // For NULL
#include <context_switch.h>

// Global variables for scheduler
Task* current_task = NULL;
Task* task_list[10]; // Simple array to hold pointers to created tasks
int num_tasks = 0;
int current_task_idx = -1; // Index of the currently running task in task_list

void scheduler_init() {
    // Initialize task list
    for (int i = 0; i < 10; ++i) {
        task_list[i] = NULL;
    }
}

// Function to add a task to the scheduler's list
void scheduler_add_task(Task* task) {
    if (num_tasks < 10) {
        task_list[num_tasks++] = task;
    }
}

void schedule() {
    if (num_tasks == 0) {
        return; // No tasks to schedule
    }

    // Simple round-robin scheduler
    current_task_idx = (current_task_idx + 1) % num_tasks;
    current_task = task_list[current_task_idx];
}

void scheduler_start() {
    // Start the first task
    scheduler_start_first_task();

    // This should not be reached
    while (1);
}
