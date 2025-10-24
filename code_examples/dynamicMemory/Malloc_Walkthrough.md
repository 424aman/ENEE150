# Memory Management in C with Structs and Dynamic Allocation

This guide demonstrates how memory is managed in C when using **structures**, **pointers**, and **dynamic allocation**.

---

## Example Code

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char *name;
};

int main() {
    // Allocate memory for a Student struct on the heap
    struct Student *s1 = malloc(sizeof(struct Student));
    if (!s1) return 1;

    // Assign primitive field (stored inside struct)
    s1->id = 101;

    // Allocate memory for name on the heap
    s1->name = malloc(20 * sizeof(char));
    if (!s1->name) {
        free(s1);
        return 1;
    }

    strcpy(s1->name, "Alice");

    printf("Student ID: %d\n", s1->id);
    printf("Student Name: %s\n", s1->name);

    // Free heap memory in reverse order of allocation
    free(s1->name);
    free(s1);

    return 0;
}
```

---

## Walkthrough of Memory Management

### 1. Before `malloc`
When the program starts, memory is divided into the following regions:

```
|---------------------------|
|        Stack              | ← local vars (none yet)
|---------------------------|
|        Heap               | ← empty
|---------------------------|
|        Data Segment       | ← globals/statics (none)
|---------------------------|
|        Text Segment       | ← compiled code
|---------------------------|
```

---

### 2. After `struct Student *s1 = malloc(sizeof(struct Student));`

`malloc` reserves space on the **heap** for one `struct Student`.
`s1` (the pointer) is stored on the **stack**, but the struct itself is on the **heap**.

```
Stack:                        Heap:
|--------------------|         |-------------------------------|
| pointer s1  -----> |-------->| struct Student { id, *name }  |
|--------------------|         |-------------------------------|
```

At this point:
- `s1` exists on the **stack**.
- The struct (`id`, `name`) lives on the **heap**.
- `id` is uninitialized.
- `name` is a NULL pointer (not allocated yet).

---

### 3. After assigning `s1->id = 101;`

The `id` field inside the heap-allocated struct now holds a value.

```
Heap memory (struct Student)
| Field | Value      | Location |
|:------|:------------|:----------|
| id    | 101         | Heap      |
| name  | (NULL)      | Heap      |
```

---

### 4. After `s1->name = malloc(20 * sizeof(char));`

Now a **second malloc** allocates memory for the student's name string.

```
Stack:                         Heap:
|--------------------|          |-------------------------------|
| s1 → 0x7ffde...   |          | struct Student {              |
|                    |          |   id = 101,                  |
|                    |          |   name → 0x55555...          |
|                    |          | }                             |
|--------------------|          |-------------------------------|
                                   |
                                   ↓
                                |----------------|
                                | name buffer    |
                                | "Alice"\0      |
                                |----------------|
```

- The pointer `s1->name` points to a **heap buffer**.
- `"Alice"` is copied into that memory via `strcpy()`.

---

### 5. Freeing Memory

At the end:
```c
free(s1->name);
free(s1);
```

Deallocation must happen in **reverse order**:
1. Free the dynamically allocated string (`s1->name`).
2. Free the struct itself (`s1`).

After freeing both, the memory becomes available again for future allocations.

---

## Key Takeaways

| Concept | Stack | Heap |
|:--------|:------|:------|
| Lifetime | Until function returns | Until freed |
| Allocation | Automatic | Manual (`malloc`, `calloc`, etc.) |
| Common errors | Stack overflow | Memory leaks, use-after-free |
| Contains | Local vars, pointers | Dynamic structs, arrays |

---

## Valgrind Output (Clean Run)

When compiled and tested with:

```bash
gcc memory_structs.c -o memory_structs
valgrind ./memory_structs
```

Expected summary:
```
HEAP SUMMARY:
    definitely lost: 0 bytes in 0 blocks
    indirectly lost: 0 bytes in 0 blocks
    still reachable: 0 bytes in 0 blocks
    suppressed: 0 bytes in 0 blocks

All heap blocks were freed -- no leaks are possible
```

---

"Always pair every malloc with a free, and never return stack addresses."
## Example Code (Incorrect Free Order)

In this version, we **free the struct before freeing the string** that belongs to it.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char *name;
};

int main() {
    struct Student *s1 = malloc(sizeof(struct Student));
    s1->id = 101;
    s1->name = malloc(20);
    strcpy(s1->name, "Alice");

    printf("%s\n", s1->name);

    
    free(s1);         // Struct is freed first
    free(s1->name);   // Then we attempt to free the pointer inside the freed struct

    return 0;
}
```

---

## Memory Map (After Wrong Free)

```
Step 1: After free(s1);
Stack:                         Heap:
|--------------------|          |-------------------------------|
| s1 (dangling ptr)  | ------X→ | struct Student { id, *name }  |  <-- freed!
|--------------------|          |-------------------------------|

Step 2: free(s1->name);
             ↑
Attempts to read pointer from invalid (freed) memory region → Undefined behavior
```

---

## Valgrind Output (Incorrect Free Order)

When compiled and run with:

```bash
gcc memory_wrong_free.c -o memory_wrong_free
valgrind ./memory_wrong_free
```

Expected output:
```
==12345== Invalid read of size 8
==12345==    at 0x4C33E4B: free (vg_replace_malloc.c:538)
==12345==    by 0x10919A: main (memory_wrong_free.c:20)
==12345==  Address 0x5204040 is 0 bytes inside a block of size 16 free'd
==12345==    at 0x4C33E4B: free (vg_replace_malloc.c:538)
==12345==    by 0x109189: main (memory_wrong_free.c:19)
==12345==  Block was alloc'd at
==12345==    at 0x4C324F3: malloc (vg_replace_malloc.c:380)
==12345==    by 0x10915B: main (memory_wrong_free.c:12)
==12345== 
==12345== Invalid free() / delete / delete[] / realloc()
==12345==    at 0x4C33E4B: free (vg_replace_malloc.c:538)
==12345==    by 0x10919A: main (memory_wrong_free.c:20)
==12345==  Address 0x5204040 is 0 bytes inside a block of size 16 free'd
==12345==    at 0x4C33E4B: free (vg_replace_malloc.c:538)
==12345==    by 0x109189: main (memory_wrong_free.c:19)
==12345== 
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 3 allocs, 3 frees, 48 bytes allocated
==12345== 
==12345== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

---

## Main thing

- **Always free inner allocations (like strings, arrays) before freeing their parent struct.**
- Accessing or freeing members of an already freed struct is **undefined behavior** and may crash or silently corrupt memory.
- Tools like **Valgrind** are essential to detect such misuse.

---

"Free in reverse order of allocation — the last allocated should be the first freed."
