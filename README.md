# Cortex-M Cycle Counter

Snippet of C code to quickly profile a block of code on a Cortex-M chip with a
CYCCNT register.

## Example

```c
//! Flag to easily disable the cycle counting
#define CORTEX_M_CYCCNT_ENABLED 1

#include "cortex_m_cyccnt.h"

int main(void) {
  // keep output state above the profiled block scope
  int result;
  // measure the long_function() call
  uint32_t cortex_m_cyccnt_profile_long_function;
  CORTEX_M_CYCCNT_WRAP(cortex_m_cyccnt_profile_long_function,
                       { result = long_function(); });

  // print result
  printf("long function cycle count: %lu\n",
         cortex_m_cyccnt_profile_long_function);

  return 0;
}
```

## Limitations

1. can't `return` inside the profiled block, bypasses the finish time check

## Other approaches

1. A much more well thought out approach using SysTick:

   https://github.com/GorgonMeducer/perf_counter

2. I think a `--wrap` approach could be really nice, since you could easily add
   instrumentation to any number of functions. Couldn't figure out how to deal with
   the wrapped function ABI though, maybe either `_Generic` or extract the ABI in a
   separate pass (eg `libabigail`).

3. `-finstrument-functions` :

   https://mcuoneclipse.com/2015/04/04/poor-mans-trace-free-of-charge-function-entryexit-trace-with-gnu-tools/
