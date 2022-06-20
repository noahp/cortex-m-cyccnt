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

static void cortexm_cyccnt_enable(void) {
  // Turn on cycle counting
  ARM_CM_DEMCR |= 1 << 24;   // enable ITM
  ARM_CM_DWT_CTRL |= 1 << 0; // enable cycle counter
}

//! Simple function to get the current cycle count value
static volatile uint32_t cortexm_cyccnt_count(void) { return ARM_CM_DWT_CYCCNT; }

//! Define a wrapper that can enclose any block (eg function body), and export a
//! variable containing the last cycle count delta for the block.
#if CORTEX_M_CYCCNT_ENABLED
#define CORTEX_M_CYCCNT_WRAP(name_, block_)                                    \
  do {                                                                         \
    extern volatile uint32_t cortex_m_cyccnt_##name_;                                   \
                                                                               \
    cortex_m_cyccnt_##name_ = cortexm_cyccnt_count();                          \
                                                                               \
    {block_}                                                                   \
                                                                               \
    cortex_m_cyccnt_##name_ =                                                  \
        cortexm_cyccnt_count() - cortex_m_cyccnt_##name_;                      \
    cortex_m_cyccnt_##name_ =                                                  \
        cortexm_cyccnt_count() - cortex_m_cyccnt_##name_;                      \
    cortex_m_cyccnt_##name_ =                                                  \
        cortexm_cyccnt_count() - cortex_m_cyccnt_##name_;                      \
  } while (0)
#else
#define CORTEX_M_CYCCNT_WRAP(name_, block_)                                    \
  do {                                                                         \
    { block_ }                                                                 \
  } while (0)
#endif
