.syntax unified
.cpu cortex-m3
.thumb

.extern current_task
.extern schedule

.global PendSV_Handler
.global scheduler_start_first_task

.thumb_func
scheduler_start_first_task:
  /* Call the scheduler to choose the first task */
  bl schedule

  /* Restore the context of the first task */
  ldr r1, =current_task
  ldr r1, [r1]
  ldr r0, [r1]      /* get the new top of stack from the TCB */

  ldmia r0!, {r4-r11} /* Restore r4-r11 from the new process stack */
  msr psp, r0       /* Set the process stack pointer */

  /* Return to the first task */
  ldr r0, =0xFFFFFFF9
  bx r0

.thumb_func
PendSV_Handler:
  /* Save the context of the current task */
  mrs r0, psp       /* Get current process stack pointer */
  stmdb r0!, {r4-r11} /* Save r4-r11 on the process stack */

  ldr r1, =current_task
  ldr r1, [r1]
  str r0, [r1]      /* Save the new top of stack in the TCB */

  /* Call the scheduler to choose the next task */
  bl schedule

  /* Restore the context of the next task */
  ldr r1, =current_task
  ldr r1, [r1]
  ldr r0, [r1]      /* get the new top of stack from the TCB */

  ldmia r0!, {r4-r11} /* Restore r4-r11 from the new process stack */
  msr psp, r0       /* Set the process stack pointer */

  /* Return to the next task */
  ldr r0, =0xFFFFFFF9
  bx r0
.size PendSV_Handler, . - PendSV_Handler
