// include/kernel/scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <task.h> // Include task.h to use Task structure

extern Task* current_task; // Pointer to the currently running task
extern Task* task_list[10]; // Simple array to hold pointers to created tasks
extern int num_tasks;      // Number of tasks currently in the system

void scheduler_init();
void scheduler_add_task(Task* task);
void schedule();
void scheduler_start(); // Function to start the scheduler

#endif // SCHEDULER_H
