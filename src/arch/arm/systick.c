// src/arch/arm/systick.c
#include <systick.h>

// SysTick Control and Status Register (STCSR)
#define STCSR_ENABLE    (1 << 0)
#define STCSR_TICKINT   (1 << 1)
#define STCSR_CLKSOURCE (1 << 2)
// SysTick Reload Value Register (STRVR)
#define STRVR_RELOAD_MASK 0x00FFFFFF

// SysTick Current Value Register (STCVR)
#define STCVR_CURRENT_MASK 0x00FFFFFF

// Interrupt Control and State Register (ICSR)
#define ICSR_PENDSVSET (1 << 28)

volatile uint32_t* const STCSR = (uint32_t*)0xE000E010;
volatile uint32_t* const STRVR = (uint32_t*)0xE000E014;
volatile uint32_t* const STCVR = (uint32_t*)0xE000E018;
volatile uint32_t* const ICSR = (uint32_t*)0xE000ED04;

void systick_init(uint32_t ticks) {
    // Disable SysTick
    *STCSR = 0;
    // Set reload register
    *STRVR = (ticks - 1) & STRVR_RELOAD_MASK;
    // Reset current value
    *STCVR = 0;
    // Enable SysTick with interrupt
    *STCSR = STCSR_ENABLE | STCSR_TICKINT | STCSR_CLKSOURCE;
}
// SysTick interrupt handler
void SysTick_Handler() {
    // Trigger PendSV exception
    *ICSR = ICSR_PENDSVSET;
}
