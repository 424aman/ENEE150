## Overview

C programs use several regions of memory:

| Region | Purpose | Example |
|:-------|:---------|:--------|
| **Text (Code)** | Stores compiled instructions | `main()` machine code |
| **Data Segment** | Stores global and static variables | `int count = 0;` |
| **Stack** | Temporary memory for function calls | Local variables |
| **Heap** | Dynamically allocated memory | `malloc()` / `free()` |

---

## The Stack

The stack is a region of memory managed automatically by the system.  
Every time a function is called, a stack frame is created to store:
- Local variables  
- Function parameters  
- Return addresses  

When the function returns, its frame is popped off the stack.

### Example
```c
void foo() {
    int x = 10;     // allocated on stack
    int y = 20;     // also on stack
}

int main() {
    foo();          // stack grows when foo() is called
    return 0;
}
```

**Characteristics**
- Managed automatically (no `malloc`/`free`)
- Fast allocation/deallocation
- Limited size (stack overflow possible with deep recursion)
- Lifetime: until the function returns

### Common Error
```c
int* make_ptr() {
    int x = 5;
    return &x; // x no longer exists after function returns
}
```
The pointer now refers to invalid stack memory — undefined behavior.

---

## The Heap

The heap is a large pool of memory for dynamic allocation —  
you manage it manually using `malloc()`, `calloc()`, `realloc()`, and `free()`.

### Example
```c
#include <stdlib.h>

int main() {
    int *arr = malloc(5 * sizeof(int)); // allocated on heap
    if (!arr) return 1;                 // always check for NULL!

    for (int i = 0; i < 5; i++) arr[i] = i;

    free(arr); // always free heap memory
    return 0;
}
```

**Characteristics**
- Manual management (must `free()` yourself)
- Slower allocation (system calls involved)
- Flexible lifetime: survives across function calls
- No automatic cleanup → possible memory leaks

### Common Mistakes
```c
int *p = malloc(sizeof(int));
*p = 42;
free(p);
*p = 99;  // use-after-free
```

---

## Comparison Summary

| Feature | **Stack** | **Heap** |
|:---------|:-----------|:-----------|
| **Managed by** | Compiler / OS | Programmer |
| **Lifetime** | Function scope | Until `free()` |
| **Speed** | Very fast | Slower |
| **Size Limit** | Usually small (MBs) | Large (depends on RAM) |
| **Errors** | Stack overflow | Memory leaks, fragmentation |
| **Use for** | Local vars, recursion | Dynamic arrays, linked lists |

---

## Visual Memory Layout

```
|---------------------------|
|        Stack (grows ↓)    | ← local vars, function calls
|---------------------------|
|        Heap (grows ↑)     | ← malloc, calloc
|---------------------------|
|     Data Segment (BSS)    | ← globals, statics
|---------------------------|
|     Text Segment (Code)   | ← compiled program
|---------------------------|
```

---

## Best Practices

- Prefer stack for small, short-lived data.
- Use heap for large, variable-sized, or persistent data.
- Always `free()` what you `malloc()`.
- Use tools like Valgrind to detect leaks or invalid memory access.
- Never return addresses of stack variables.

---

## See Also

- [Valgrind Memory Checker](https://valgrind.org/)
- [malloc(3) man page](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [C Memory Layout Diagram (MIT)](https://people.csail.mit.edu/bratus/courses/2016s-CSCI-3730/readings/memory.pdf)

---

"Memory management is where C becomes both powerful and dangerous."
