//! Cycle counter for Cortex-M

#pragma once

#include <stdint.h>

//! By default the CORTEX_M_CYCCNT_WRAP() instrumentation will be inactive,
//! enable it by defining this to 1 before including the header
#if !defined(CORTEX_M_CYCCNT_ENABLED)
#define CORTEX_M_CYCCNT_ENABLED 0
#endif

//! From:
//! https://www.embedded-computing.com/articles/measuring-code-execution-time-on-arm-cortex-m-mcus
#if !defined(CORTEX_M_CYCNT_TEST)
#define ARM_CM_DEMCR (*(uint32_t *)0xE000EDFC)
#define ARM_CM_DWT_CTRL (*(uint32_t *)0xE0001000)
#define ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)
#endif

//! Run this once at system init, before doing any profiling
static void cortexm_cyccnt_enable(void) {
  // Turn on cycle counting
  ARM_CM_DEMCR |= 1 << 24;   // enable ITM
  ARM_CM_DWT_CTRL |= 1 << 0; // enable cycle counter
}

//! Simple function to get the current cycle count value
static uint32_t cortexm_cyccnt_count(void) { return ARM_CM_DWT_CYCCNT; }

//! Define a wrapper, CORTEX_M_CYCCNT_WRAP_STORE,  that can enclose any block
//! (eg function body), and export a variable containing the last cycle count
//! delta for the block.
//!
//! Outputs the count to the first argument.
//!
//! NOTE: the enclosed block shouldn't contain any `return` statements, they'll
//! skip over the final count value which will result in a garbage count.
#if CORTEX_M_CYCCNT_ENABLED
#define CORTEX_M_CYCCNT_WRAP(result_, block_)                                  \
  do {                                                                         \
    result_ = cortexm_cyccnt_count();                                          \
                                                                               \
    {block_}                                                                   \
                                                                               \
    result_ = cortexm_cyccnt_count() - result_;                                \
  } while (0)
#else
#define CORTEX_M_CYCCNT_WRAP(name_, result_, block_)                           \
  do {                                                                         \
    { block_ }                                                                 \
  } while (0)
#endif
