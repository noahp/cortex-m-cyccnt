#define CORTEX_M_CYCNT_TEST

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// https://stackoverflow.com/a/13772771
#ifdef _WIN32
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

//! For testing, override these
static uint32_t test_ARM_CM_DEMCR;
static uint32_t test_ARM_CM_DWT_CTRL;
static volatile uint32_t test_ARM_CM_DWT_CYCCNT(void) {
    return (uint32_t)__rdtsc();
}

#define ARM_CM_DEMCR test_ARM_CM_DEMCR
#define ARM_CM_DWT_CTRL test_ARM_CM_DWT_CTRL
#define ARM_CM_DWT_CYCCNT test_ARM_CM_DWT_CYCCNT()

#define CORTEX_M_CYCCNT_ENABLED 1

#include "cortex_m_cyccnt.h"

static volatile uint32_t cortex_m_cyccnt_some_function_prof;
static int some_function(float a, float b) {
  CORTEX_M_CYCCNT_WRAP(some_function_prof, {
    if (a > b) {
      return a * b;
    } else {
      return b / a;
    }
  });
}

int main(void) {
  uint32_t start = (uint32_t)__rdtsc();
  printf("%d\n", some_function(1.0f, 2.0f));
  printf("counts: %" PRIu32 "\n", cortex_m_cyccnt_some_function_prof);

  start = (uint32_t)__rdtsc() - start;
  printf("total: %" PRIu32 "\n", start);

  return 0;
}
