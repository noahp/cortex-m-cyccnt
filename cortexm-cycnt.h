//! Cycle counter for Cortex-M

#include <stdint.h>

//! From:
//! https://www.embedded-computing.com/articles/measuring-code-execution-time-on-arm-cortex-m-mcus
#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

// Array of cycle counts. Exposed to the linker so you can dump it easily from
// gdb from other frames
extern volatile uint32_t cycle_count[100];
volatile uint32_t cycle_count[100] = {0};

static uint32_t start = 0;
static void prv_start_cycle_count(void) {
  // Turn on cycle counting
  ARM_CM_DEMCR |= 1 << 24;  // enable ITM
  ARM_CM_DWT_CTRL |= 1 << 0;   // enable cycle counter

  // save starting count
  start = ARM_CM_DWT_CYCCNT;
}

static void prv_finish_cycle_count(void) {
  static uint32_t cycle_count_idx = 0;

  uint32_t end = ARM_CM_DWT_CYCCNT;
  uint32_t delta;

  // unsigned math handles (one) wrap around
  delta = end - start;

  cycle_count[cycle_count_idx] = delta;
  cycle_count_idx = (cycle_count_idx + 1) % 100;

  // TODO disable cycle counting now? or save + restore to previous position...
  // ARM_CM_DWT_CTRL &= ^(1 << 24);
}
