/* src/arch/arm/startup.s */
.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler
.global _sdata
.global _edata
.global _sbss
.global _ebss
.global _etext
.global _estack

.section .isr_vector, "a", %progbits
.word _estack
.word Reset_Handler + 1
.word 0 /* NMI_Handler */
.word HardFault_Handler + 1
.word 0 /* MemManage_Handler */
.word 0 /* BusFault_Handler */
.word 0 /* UsageFault_Handler */
.word 0
.word 0
.word 0
.word 0
.word 0 /* SVCall_Handler */
.word 0 /* DebugMon_Handler */
.word 0
.word PendSV_Handler + 1
.word SysTick_Handler + 1

.section .text

.thumb_func
HardFault_Handler:
  b .

.thumb_func
Reset_Handler:
  /* Set the stack pointer */
  ldr r0, =_estack
  mov sp, r0

  /* Copy .data section from flash to RAM */
  ldr r0, =_etext  /* source */
  ldr r1, =_sdata  /* destination */
  ldr r2, =_edata  /* end of .data */
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, [r0]
  str r3, [r1]
  adds r0, r0, #4
  adds r1, r1, #4

LoopCopyDataInit:
  cmp r1, r2
  bcc CopyDataInit

  /* Zero out .bss section */
  ldr r1, =_sbss
  ldr r2, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str r3, [r1]
  adds r1, r1, #4

LoopFillZerobss:
  cmp r1, r2
  bcc FillZerobss

  /* Call the C language main function */
  bl main

  /* Trap the processor in a loop if main returns */
  b .

.size Reset_Handler, . - Reset_Handler

.thumb_func
.weak SysTick_Handler
SysTick_Handler:
  b .

.thumb_func
.weak PendSV_Handler
PendSV_Handler:
  b .
