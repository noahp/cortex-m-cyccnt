//! Cycle counter for Cortex-M

#include <stdint.h>

//! From:
//! https://www.embedded-computing.com/articles/measuring-code-execution-time-on-arm-cortex-m-mcus
#define  ARM_CM_DEMCR      (*(volatile uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(volatile uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)

static uint32_t prv_start_cycle_count(void) {
  // Turn on cycle counting
  ARM_CM_DEMCR |= 1 << 24;  // enable ITM
  ARM_CM_DWT_CTRL |= 1 << 0;   // enable cycle counter
  return ARM_CM_DWT_CYCCNT;
}

static uint32_t prv_finish_cycle_count(uint32_t startcount) {
  uint32_t end = ARM_CM_DWT_CYCCNT;

  // TODO disable cycle counting now? or save + restore to previous position...
  // ARM_CM_DWT_CTRL &= ^(1 << 24);

  // unsigned math handles (one) wrap around
  return end - startcount;
}
