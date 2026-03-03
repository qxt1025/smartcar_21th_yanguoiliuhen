---
name: stc32g144k246-c51-firmware
description: Provide C51-compatible coding standards for STC32G144K246 firmware projects, including function header comments, variable purpose comments, key-statement comments, ISR rules, variable-footprint minimization, and runtime-performance optimization. Use when writing or modifying code in this project for STC32G144K246 and when strict comment quality, low memory use, and high execution speed are required.
---

# STC32G144K246 C51 Firmware

## Overview

Produce maintainable firmware code that compiles under C51 constraints in STC32G144K246 projects.
Prioritize correctness, low variable footprint, and fast execution before feature expansion.

## Workflow

1. Confirm target files and module boundaries before editing.
2. Reuse existing macros, typedefs, and driver interfaces before adding new abstractions.
3. Design data flow to minimize variable count and temporary storage.
4. Write code with strict C51 compatibility, then add required comments.
5. Check ISR safety, array bounds, integer ranges, and timing-sensitive statements.

## Performance and Memory Rules

- Minimize variable count in each function; delete unused and redundant temporaries.
- Prefer the narrowest safe type for each variable to reduce RAM usage.
- Reuse local variables only when lifetimes do not overlap and readability remains clear.
- Avoid copying buffers unless required by correctness or DMA constraints.
- Keep hot-path code branch-light and loop-light; avoid unnecessary work inside loops.
- Move constant calculations out of loops and precompute reusable values.
- Prefer integer arithmetic and fixed-point scaling over floating-point operations.
- Avoid division and modulo in high-frequency paths when equivalent multiply/shift logic is practical.
- Keep ISR execution time minimal; only capture state and defer heavy work to main loop.
- Avoid function-call overhead in tight paths when a small static inline-style helper or macro is clearly faster and safe in this codebase.
- When optimization conflicts occur, prioritize runtime speed first, then reduce variable/storage overhead without changing behavior.

## C51 Constraints

- Use C51-compatible syntax only; avoid C99/C11-only language features.
- Declare local variables at the start of each function or block.
- Avoid `for (int i = 0; ...)`; declare index variables first.
- Prefer project-defined types; otherwise use C51-friendly base types (`unsigned char`, `unsigned int`, `unsigned long`).
- Avoid dynamic allocation, recursion, and large stack objects.
- Keep ISR code short and deterministic.
- Add comments for hardware-related magic numbers and thresholds.

## Comment Rules

### Function Header

Add a header comment for every new or rewritten function. Include purpose, parameters, return value, and usage notes.

```c
/*
 * Purpose: Scan key input with fixed-period debounce
 * Param  : sample_ms - sampling period in milliseconds
 * Return : 1 means valid press, 0 means not pressed
 * Note   : Active-low input; call period must match sample_ms
 */
unsigned char key_scan(unsigned int sample_ms)
{
    unsigned char key_raw = 0;      /* Raw key pin level */
    unsigned char key_state = 0;    /* Debounced stable state */

    key_raw = KEY_PIN;              /* Read key pin level */
    if(key_raw == 0)                /* Active low means pressed */
    {
        key_state = 1;              /* Mark valid pressed state */
    }

    return key_state;               /* Return stable key result */
}
```

### Variable Comment

- Add short comments after variable declarations to explain role, unit, or valid range.
- For shared globals, state read/write ownership and ISR/main-loop interaction.

### Important Statement Comment

- Add comments after key statements to explain intent, not literal syntax.
- Always comment:
- Register setup
- Interrupt enable/disable
- Delay or timer parameter calculations
- Threshold checks and clamping
- State machine transitions

## ISR Rules

- State ISR trigger source and purpose clearly.
- Keep ISR work limited to sampling, flag updates, and interrupt clearing.
- Move complex logic to main loop/task functions.
- Keep shared variable access predictable and minimal.

## Output Checklist

- Confirm C51 syntax compatibility.
- Confirm variable footprint is minimized (type width, count, temporary storage).
- Confirm runtime hot paths are optimized (loop body, arithmetic, branching, ISR load).
- Confirm function header comments are present.
- Confirm key variable roles are documented.
- Confirm key statements include intent comments.
- Confirm ISR code avoids heavy processing.
