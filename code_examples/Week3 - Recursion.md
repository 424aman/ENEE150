# ENEE150 Recitation — Recursion

## Project 1 Reminder

Please read the **Project 1 description carefully over the weekend**.

As you read it:

- Make sure you understand what the program is supposed to do.
- Pay attention to the required input/output format.
- Write down any questions you have.
- Bring your questions to **Tuesday's lecture**.

---

# Quiz Practice: `recursion_loop.c`

This is a previous quiz-style problem. You may see questions similar to this on **Quiz 1**.

The original code contains:

- `f1()`, which uses recursion
- `f2()`, which uses a `while` loop

Your goal is to rewrite them so that:

- `f1()` **does not use recursion**
- `f2()` **does use recursion**

You may only change the implementations of `f1()` and `f2()`.

## Original `f1()`

```c
int f1(void)
{
    int tmp;

    printf("Enter a positive integer: ");
    scanf("%d", &tmp);

    if (tmp > 0)
        return tmp;
    else
        f1();
}
```

### What does it do?

`f1()` keeps asking the user for an integer until the user enters a **positive integer**.

If the number is not positive, the function calls itself again.

### Hint 1

What condition causes `f1()` to stop calling itself?

It stops when:

```c
tmp > 0
```

### Hint 2

If you need to repeat something until a condition becomes true, but **cannot use recursion**, what other C construct can repeat code?

Think about a `while` loop.

### Solution

```c
int f1(void)
{
    int tmp = 0;

    while (tmp <= 0)
    {
        printf("Enter a positive integer: ");
        scanf("%d", &tmp);
    }

    return tmp;
}
```

Now the loop repeats until the user enters a positive integer.

---

## Original `f2()`

```c
void f2(int n)
{
    while (n > 1)
    {
        printf("%d -> ", n);

        if (n % 2 == 0)
            n = n / 2;
        else
            n++;
    }

    printf("%d\n", n);

    return;
}
```

### What does it do?

The function repeatedly changes `n` until it reaches `1`.

If `n` is even:

```c
n = n / 2;
```

If `n` is odd:

```c
n++;
```

For example:

```text
7 -> 8 -> 4 -> 2 -> 1
```

### Hint 1

A recursive function needs:

1. A **base case** that stops recursion.
2. A **recursive call** that gets closer to the base case.

Here, the loop stops when `n` reaches `1`, so that should become the base case.

### Hint 2

Instead of changing `n` and going back to the top of a loop, call `f2()` again with the **new value of `n`**.

For example:

```c
f2(n / 2);
```

or

```c
f2(n + 1);
```

### Solution

```c
void f2(int n)
{
    if (n <= 1)
    {
        printf("%d\n", n);
        return;
    }

    printf("%d -> ", n);

    if (n % 2 == 0)
        f2(n / 2);
    else
        f2(n + 1);
}
```

The recursive call replaces the repetition that was previously handled by the `while` loop.

---

# `implicitRecursion.c`

This program demonstrates **indirect recursion**.

Normally, recursion looks like this:

```text
f() -> f()
```

But functions can also call each other in a cycle:

```text
main() -> f() -> g() -> main()
```

That is still recursion even though none of the functions directly calls itself.

---

## Part A — Original Program

The important calls are:

```c
main()
{
    ...
    if (cnt <= 2)
        f();
}

void f(void)
{
    printf("In f():\n");
    g();
}

void g(void)
{
    printf("In g():\n");
    main();
}
```

So the call sequence is:

```text
main()
   ↓
f()
   ↓
g()
   ↓
main()
   ↓
f()
   ↓
g()
   ↓
main()
```

The important variable is:

```c
static int cnt = 0;
```

Every time `main()` runs:

```c
cnt++;
```

Because `cnt` is `static`, its value is **preserved between calls to `main()`**.

### Predict the output

First call:

```text
cnt = 1
```

Since `1 <= 2`, `main()` calls `f()`.

Then:

```text
f() -> g() -> main()
```

The second call to `main()` makes:

```text
cnt = 2
```

It calls `f()` again.

Eventually the third call to `main()` makes:

```text
cnt = 3
```

Now `3 <= 2` is false, so the recursion stops.

### Output

```text
In main():
counter =   1
In f():
In g():
In main():
counter =   2
In f():
In g():
In main():
counter =   3
```

---

# Part B — Adding Another Call to `g()`

In the version of the file shown here, the commented lines are inside `main()`:

```c
f();
printf("I am here calling g() \n");
g();
```

Now, whenever `cnt <= 2`, `main()` calls both `f()` and later `g()`.

Remember that `f()` already calls `g()`:

```text
f() -> g() -> main()
```

The important thing is that execution returns to the previous function after the deeper recursive call finishes.

A simplified call sequence looks like:

```text
main (cnt = 1)
    f
        g
            main (cnt = 2)
                f
                    g
                        main (cnt = 3)
                print "I am here..."
                g
                    main (cnt = 4)
    print "I am here..."
    g
        main (cnt = 5)
```

### Predicted output

```text
In main():
counter =   1
In f():
In g():
In main():
counter =   2
In f():
In g():
In main():
counter =   3
I am here calling g()
In g():
In main():
counter =   4
I am here calling g()
In g():
In main():
counter =   5
```

Notice that even after `cnt` reaches `3`, earlier calls to `main()` are still waiting to finish.

When execution returns to them, they continue with the next statements after:

```c
f();
```

---

# Part C — Why Is `cnt` Static?

The program declares:

```c
static int cnt = 0;
```

A local `static` variable is initialized only once and keeps its value between function calls.

So the recursive calls see:

```text
1
2
3
```

instead of starting over each time.

If we changed it to:

```c
int cnt = 0;
```

then every new call to `main()` would create a new `cnt` and initialize it to `0`.

Every call would therefore do:

```text
cnt = 0
cnt++
cnt = 1
```

Then:

```c
if (cnt <= 2)
```

would always be true.

The call sequence would continue:

```text
main()
→ f()
→ g()
→ main()
→ f()
→ g()
→ main()
→ ...
```

Eventually the program would likely terminate because it **runs out of stack space**, causing a stack overflow.

---

# Main Ideas to Know

### Direct Recursion

A function calls itself:

```c
void f()
{
    f();
}
```

### Indirect Recursion

Functions call each other in a cycle:

```text
f() -> g() -> f()
```

or, in this example:

```text
main() -> f() -> g() -> main()
```

### Base Case

Recursive code needs some condition that eventually stops further recursive calls.

For `implicitRecursion.c`, the stopping condition is effectively:

```c
cnt > 2
```

### Static Local Variables

```c
static int cnt = 0;
```

means:

- `cnt` is local to the function.
- It is initialized only once.
- Its value survives between function calls.

### Recursion vs. Loops

Many recursive operations can be rewritten using loops, and many loops can be rewritten using recursion.

For the quiz problem:

```text
f1: recursion -> loop
f2: loop -> recursion
```

The key is to first understand **what causes the repetition** and **what causes it to stop**.
