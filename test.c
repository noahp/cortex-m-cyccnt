
#include <inttypes.h>
#include <math.h>
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
static uint32_t test_ARM_CM_DWT_CYCCNT(void) { return (uint32_t)__rdtsc(); }

#define ARM_CM_DEMCR test_ARM_CM_DEMCR
#define ARM_CM_DWT_CTRL test_ARM_CM_DWT_CTRL
#define ARM_CM_DWT_CYCCNT test_ARM_CM_DWT_CYCCNT()

#define CORTEX_M_CYCNT_TEST
#define CORTEX_M_CYCCNT_ENABLED 1

#include "cortex_m_cyccnt.h"

static uint32_t cortex_m_cyccnt_some_function_prof;
static float some_function(float a, float b) {
  float result;
  CORTEX_M_CYCCNT_WRAP(cortex_m_cyccnt_some_function_prof, {
    if (a > b) {
      result = sinf(a) + cosf(b);
    } else {
      result = cosf(a) + sinf(b);
    }
  });
  return result;
}

//! -finstrument-functions example (heavy ish)
extern void __cyg_profile_func_enter(void *this_fn, void *call_site);
extern void __cyg_profile_func_exit(void *this_fn, void *call_site);

__attribute__((no_instrument_function)) void
__cyg_profile_func_enter(void *this_fn, void *call_site) {
  (void)call_site;
  printf("Entering %p\n", this_fn);
}
__attribute__((no_instrument_function)) void
__cyg_profile_func_exit(void *this_fn, void *call_site) {
  (void)call_site;
  printf("Exiting %p\n", this_fn);
}

int main(void) {
  cortexm_cyccnt_enable();

  uint32_t cortex_m_cyccnt_main;

  CORTEX_M_CYCCNT_WRAP(cortex_m_cyccnt_main, {
    printf("some_function result: %.03f\n", (double)some_function(1.0f, 2.0f));
    printf("cortex_m_cyccnt_some_function_prof: %" PRIu32 "\n",
           cortex_m_cyccnt_some_function_prof);
  });
  printf("cortex_m_cyccnt_main: %" PRIu32 "\n", cortex_m_cyccnt_main);

  return 0;
}
