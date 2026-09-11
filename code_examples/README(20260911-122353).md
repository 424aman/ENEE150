# ENEE150 C Examples — Interactive Walkthrough

This README is designed to be used **interactively in class**. For each example:

1. **Read the code before running it.**
2. **Predict what it will do.**
3. Trace the important variables by hand.
4. Compile it.
5. Run it.
6. Compare the real result with the prediction.
7. Change something small and predict again.

> The goal is not just to get the right output. The goal is to understand **why control flows to a particular line** and **which variable a statement is referring to**.

---

# 1. `scope.c` — Scope, Local Variables, Global Variables, and `static`

## Starting code

```c
/* This program demonstrates the scope of variables.
 * for each of the 4 cases in showvalue(), predict the output and then
 * change the code, compile, run and see the real output.
 *                                                        Gang Qu
 *                                                        Jan. 24, 2024
 */

#include <stdio.h>

int a = 1;
int b = 12;

void showvalue(void);

int main(void)
{
    int i;

    for (i = 10; i <= b; i++)
    {
        a++;
        printf("in main(), a = %d, \n", a);
        showvalue();
    }

    return 0;
}

void showvalue()
{
    static int a = 3;     // case 0:
                          // case 1: remove static
                          // case 2: remove the entire statement
    static int i = 0;     // case 3: remove static

    a = a + b;
    printf("Times function is called: %d, a = %d\n", ++i, a);

    // What will happen if ++i is changed to i++?
}
```

Before starting this section, restore the original file:

```bash
cp scope.bak scope.c
```

If two TAs are working in the same directory at the same time, use different filenames, for example:

```bash
cp scope.bak scope1.c
```

Compile and run:

```bash
gcc scope.c -o scope
./scope
```

---

## First: identify every variable

There are **two different variables named `a`** in Case 0:

```c
int a = 1;
```

This is the **global `a`**.

Inside `showvalue()`:

```c
static int a = 3;
```

This is a **local `a`**. It hides the global `a` while execution is inside `showvalue()`.

There are also two different variables named `i`:

```c
int i;
```

inside `main()`, and

```c
static int i = 0;
```

inside `showvalue()`.

They are completely separate variables.

---

## How many times does the loop run?

The loop is:

```c
for (i = 10; i <= b; i++)
```

and:

```c
b = 12;
```

So `main()` uses:

| Iteration | `i` |
|---|---:|
| 1 | 10 |
| 2 | 11 |
| 3 | 12 |

Then `i` becomes 13, and:

```c
13 <= 12
```

is false.

Therefore `showvalue()` is called **3 times**.

---

# Case 0 — Original Code

```c
static int a = 3;
static int i = 0;
```

## Predict before running

Ask:

- Which `a` does `main()` modify?
- Which `a` does `showvalue()` modify?
- Does the local `a` keep its value after the function returns?
- Does the local `i` keep its value after the function returns?

Because both variables in `showvalue()` are `static`, they are initialized only once and **retain their values between function calls**.

### Trace

| Call | Global `a` after `a++` in `main` | Static local `a` before `+ b` | Static local `a` after `+ 12` | Static `i` printed |
|---|---:|---:|---:|---:|
| 1 | 2 | 3 | 15 | 1 |
| 2 | 3 | 15 | 27 | 2 |
| 3 | 4 | 27 | 39 | 3 |

<details>
<summary><strong>Expected output</strong></summary>

```text
in main(), a = 2,
Times function is called: 1, a = 15
in main(), a = 3,
Times function is called: 2, a = 27
in main(), a = 4,
Times function is called: 3, a = 39
```

</details>

### Main idea

`main()` changes the **global** `a`.

`showvalue()` changes its own **local static** `a`.

The two variables happen to have the same name, but they are different objects.

---

# Case 1 — Remove `static` from `a`

Change:

```c
static int a = 3;
```

to:

```c
int a = 3;
```

Leave:

```c
static int i = 0;
```

unchanged.

## Predict

Now `a` is an ordinary local variable.

Every time `showvalue()` begins:

```c
int a = 3;
```

creates/initializes a new local `a`.

So every call does:

```text
a = 3
a = a + 12
a = 15
```

But `i` is still static, so the call counter continues to increase.

<details>
<summary><strong>Expected output</strong></summary>

```text
in main(), a = 2,
Times function is called: 1, a = 15
in main(), a = 3,
Times function is called: 2, a = 15
in main(), a = 4,
Times function is called: 3, a = 15
```

</details>

### Key question

Why is the printed `a` always 15?

Because the local variable is initialized to 3 **again on every function call**.

---

# Case 2 — Remove the local declaration of `a`

Remove this line completely:

```c
static int a = 3;
```

Now there is no local `a` inside `showvalue()`.

Therefore this line:

```c
a = a + b;
```

uses the **global `a`**.

That means `main()` and `showvalue()` are now modifying the **same variable**.

## Trace

Initial:

```text
global a = 1
```

### Iteration 1

`main()`:

```c
a++;
```

so:

```text
a = 2
```

Then `showvalue()`:

```c
a = a + 12;
```

so:

```text
a = 14
```

### Iteration 2

Back in `main()`:

```c
a++;
```

so:

```text
a = 15
```

Then:

```text
a = 15 + 12 = 27
```

### Iteration 3

Back in `main()`:

```text
a = 28
```

Then `showvalue()`:

```text
a = 40
```

<details>
<summary><strong>Expected output</strong></summary>

```text
in main(), a = 2,
Times function is called: 1, a = 14
in main(), a = 15,
Times function is called: 2, a = 27
in main(), a = 28,
Times function is called: 3, a = 40
```

</details>

### Important idea

A local declaration can **hide** a global variable.

Once the local declaration is removed, references to `a` inside `showvalue()` resolve to the global `a`.

---

# Case 3 — Case 0, but remove `static` from `i`

Restore Case 0 first:

```bash
cp scope.bak scope.c
```

Then change:

```c
static int i = 0;
```

to:

```c
int i = 0;
```

Keep:

```c
static int a = 3;
```

## Predict

The static local `a` still remembers:

```text
3 -> 15 -> 27 -> 39
```

But `i` is recreated every time `showvalue()` begins:

```c
int i = 0;
```

Then:

```c
++i
```

changes it to 1.

So every function call says it is call number 1.

<details>
<summary><strong>Expected output</strong></summary>

```text
in main(), a = 2,
Times function is called: 1, a = 15
in main(), a = 3,
Times function is called: 1, a = 27
in main(), a = 4,
Times function is called: 1, a = 39
```

</details>

---

## `++i` versus `i++`

Current code:

```c
printf("Times function is called: %d, a = %d\n", ++i, a);
```

`++i` means:

1. increment `i`
2. use the new value

If:

```text
i = 0
```

then:

```c
++i
```

produces 1.

If it is changed to:

```c
i++
```

then C:

1. uses the current value
2. increments afterward

So with a static `i`, the displayed call numbers would begin:

```text
0
1
2
```

instead of:

```text
1
2
3
```

---

## `scope.c` summary

| Case | Local `a` | Local `i` | Does `a` retain value? | Does call count retain value? |
|---|---|---|---|---|
| 0 | `static int a = 3` | `static int i = 0` | Yes | Yes |
| 1 | `int a = 3` | `static int i = 0` | No | Yes |
| 2 | No local `a` | `static int i = 0` | Uses global `a` | Yes |
| 3 | `static int a = 3` | `int i = 0` | Yes | No |

---

# 2. `scanf.c` — Reading Different Types of Input

## Code

```c
/* This progam shows the basic features of scanf()

                                                        Gang Qu
                                                        Sept. 1, 2015
*/

#include <stdio.h>

int main(void)
{
    int a;
    float b;
    char c;
    char name[10], name2[10];

    // scanf("%d %f %c %s", &a, &b, &c, name);

    scanf("%d %f %c %[a-zA-Z]s", &a, &b, &c, name);

    printf("a= %d, b = %f, c = %c, name = %s\n",
           a, b, c, name);

    scanf("%s", name2);
    printf("%s\n", name2);

    /*
    printf("Enter in the following format: \n \t student_ID ... score ... \n");
    scanf("%*s %d %*s %c", &a, &c);

    printf("a = %d, c = %c\n", a, c);
    scanf("%d", &a);

    scanf("%*c %c", &c);
    printf("%c \n", c);
    */

    return 0;
}
```

Compile:

```bash
gcc scanf.c -o scanf
./scanf
```

---

## Format specifiers

This line:

```c
scanf("%d %f %c %[a-zA-Z]s", &a, &b, &c, name);
```

contains several different conversions.

| Format | Reads | Variable |
|---|---|---|
| `%d` | integer | `a` |
| `%f` | floating-point number | `b` |
| `%c` | one character | `c` |
| `%[a-zA-Z]` | sequence containing letters A-Z or a-z | `name` |

For ordinary variables, `scanf()` needs their **addresses**:

```c
&a
&b
&c
```

Arrays such as:

```c
char name[10];
```

already decay to a pointer to their first element in this context, so we write:

```c
name
```

rather than:

```c
&name
```

---

## Try an input

For example:

```text
25 93.5 A Alice
```

Before running, predict:

```text
a = ?
b = ?
c = ?
name = ?
```

Then run the program and check.

---

## A useful detail about `%c`

Unlike `%d` and `%f`, `%c` does **not automatically skip whitespace**.

That is one reason you will commonly see:

```c
scanf(" %c", &c);
```

Notice the space before `%c`.

That space tells `scanf()` to skip whitespace before reading the character.

---

## The scanset

This part:

```c
%[a-zA-Z]
```

means:

> Keep reading characters while they are uppercase or lowercase letters.

This is different from:

```c
%s
```

because `%s` reads non-whitespace characters.

### Small source-code quirk

The provided program uses:

```c
%[a-zA-Z]s
```

The scanset itself ends at `]`; the trailing `s` is therefore treated as a **literal `s` to match**, not as part of the scanset conversion.

For demonstrating a scanset by itself, the usual form is:

```c
scanf("%d %f %c %[a-zA-Z]", &a, &b, &c, name);
```

For class, it is useful to point out exactly what the provided code says and let students experiment with it.

---

## Suppressing input with `*`

Inside the commented section:

```c
scanf("%*s %d %*s %c", &a, &c);
```

`*` means:

> Read this input, but do not store it.

For example:

```c
%*s
```

reads and discards a string.

This is useful when input contains words or separators you do not need.

---

## Interactive questions

Before showing the answer, ask:

1. Why do `a`, `b`, and `c` use `&`?
2. Why does `name` not need `&`?
3. What does `%[a-zA-Z]` accept?
4. Does `%c` skip a leftover newline automatically?
5. What does `*` do inside a `scanf()` format string?
6. What would change if `%c` became `" %c"`?

---

# 3. `scanf_printf.c` — Input and Output Together

## Code

```c
/* This program reads a student ID (int), a score (float), a grade
        (char), and prints them out.
   There are two implementations, one tries to prompt the user for data
        piece by piece, the other tries to get all at one time. Think
        about their advantages and disadvantages. Also note how to read
        in a character in the first implementation.

                                                        Gang Qu
                                                        Sept. 1, 2015
*/

#include <stdio.h>

int main(void)
{
    int SID;
    float Score;
    char Grade;

    /*
    printf("Enter the student ID: ");
    scanf("%d", &SID);

    printf("Enter the score: ");
    scanf("%f", &Score);

    printf("Enter the grade: ");
    scanf("%c%c", &Grade, &Grade);
    // note that the last statement reads in two characters
    */

    /*
       The following two statements achieve the same thing as the above
       statements.

       Note that in this case we only need to read variable Grade once
       and the white space will be skipped.
    */

    printf("Enter the student ID, score, and grade: ");
    scanf("%d %f %c", &SID, &Score, &Grade);

    printf("SID: %d \nScore: %f \nGrade: %c\n",
           SID, Score, Grade);

    return 0;
}
```

Compile:

```bash
gcc scanf_printf.c -o scanf_printf
./scanf_printf
```

---

## What the program stores

```c
int SID;
float Score;
char Grade;
```

Example input:

```text
123456789 92.5 A
```

creates approximately:

```text
SID   = 123456789
Score = 92.5
Grade = 'A'
```

Then:

```c
printf("SID: %d \nScore: %f \nGrade: %c\n",
       SID, Score, Grade);
```

prints them using matching format specifiers.

---

## Why does this work?

```c
scanf("%d %f %c", &SID, &Score, &Grade);
```

The spaces in the format string are important.

Whitespace in a `scanf()` format tells `scanf()` to consume any amount of whitespace before the next conversion.

Therefore the space before:

```c
%c
```

prevents the previous newline or space from accidentally becoming the grade.

---

## Compare the two approaches

### Approach A — Ask one item at a time

```c
printf("Enter the student ID: ");
scanf("%d", &SID);

printf("Enter the score: ");
scanf("%f", &Score);

printf("Enter the grade: ");
...
```

Advantages:

- Friendly prompts
- Easier for a user to understand
- Easier to identify which input is wrong

Potential issue:

- Character input requires care because a newline may remain in the input buffer.

---

### Approach B — Read everything together

```c
printf("Enter the student ID, score, and grade: ");
scanf("%d %f %c", &SID, &Score, &Grade);
```

Advantages:

- Shorter
- Convenient when the input format is known
- The whitespace in the format naturally skips separators

Potential disadvantage:

- The user must know the expected order.

---

## Why does the old version use this?

```c
scanf("%c%c", &Grade, &Grade);
```

The intent is to read two characters:

1. the leftover newline
2. the actual grade

Both are stored into the same variable, so the second character overwrites the first.

A clearer modern version would normally be:

```c
scanf(" %c", &Grade);
```

The leading space tells `scanf()` to skip whitespace first.

---

## Interactive trace

Input:

```text
1001 83.7 B
```

Fill this out **before running**:

| Step | Variable | Value |
|---|---|---|
| `%d` | `SID` | ? |
| `%f` | `Score` | ? |
| `%c` | `Grade` | ? |

Then compile and verify.

---

# 4. `printf.c` — Useful `printf()` Tricks

## Code

```c
/* This programs shows all (probably more than that) you need to know about
        printf().
   data types have to be introduced before this program.

                                                        Gang Qu
                                                        Feb. 11, 2011
*/

#include <stdio.h>

int main(void)
{
    int a1 = 23, a2 = -19;
    short int b1 = 53, b2 = 12;
    int i1 = 32000, i2 = -320;

    float f1 = 1.0, f2 = -0.0001, f3 = 2.3e+8, f4 = -1.23E-02;

    // integer example
    printf("%%d: a1 = %d, a2 = %d, b1 = %d, b2 = %d, i1 = %d, i2 = %d\n",
            a1, a2, b1, b2, i1, i2);
    printf("%%i: a1 = %i, a2 = %i\n", a1, a2);
    printf("%%x: a1 = %x, a2 = %x\n", a1, a2);
    printf("%%X: a1 = %X, a2 = %X\n", a1, a2);
    printf("%%o: b1 = %o, b2 = %o\n", b1, b2);
    printf("%%O: b1 = %O, b2 = %O\n", b1, b2);
    printf("%%u: i1 = %u, i2 = %u\n", i1, i2);
    printf("%%U: i1 = %U, i2 = %U\n", i1, i2);

    a1 = 023;
    b1 = 0x53;

    printf("\nAfter reassign values:\n a1: 023=%d, b1: 0x53=%d\n",
           a1, b1);

    // floating point example
    printf("\n\n");
    printf("1.0 = %f, -0.0001 = %f\n", f1, f2);
    printf("2.3e+8 = %f, -1.23E-02 = %f\n\n", f3, f4);

    printf("1.0 = %e, -0.0001 = %e\n", f1, f2);
    printf("2.3e+8 = %e, -1.23E-02 = %e\n\n", f3, f4);

    printf("1.0 = %E, -0.0001 = %E\n", f1, f2);
    printf("2.3e+8 = %E, -1.23E-02 = %E\n\n", f3, f4);

    printf("1.0 = %g, -0.0001 = %g\n", f1, f2);
    printf("2.3e+8 = %g, -1.23E-02 = %g\n\n", f3, f4);

    printf("1.0 = %G, -0.0001 = %G\n", f1, f2);
    printf("2.3e+8 = %G, -1.23E-02 = %G\n\n", f3, f4);

    // char and string example
    printf("%c, %c, %c, \n", '&', 0x4B, 87);

    printf("%s%s\n", "Hello \n World\0 Welcome!", "!");
    printf("%s%s\n", "Hello World Welcome!", "!");

    // field width example
    printf("%12d \n%12f \n%14e \n%6E \n%012d\n",
           23, 3.1416, -2.0, 3.1, -23);

    printf("%012d \n%*d \n%0*d \n%*d \n",
           -23, 12, 23, 12, 23, 012, 23);

    // precision example
    printf("%5.2f \n%.2f \n%9.2e \n",
           3.14159, 3.14159, 13.14159);

    printf("%*.*f \n", 5, 3, 3.14159);

    printf("%5.2d \n%10.5s\n", 1, "Hello, World!");

    // format example
    printf("%-15ftest \n%+ftest \n% ftest\n",
           3.1416, 3.1416, 3.1416);

    return 0;
}
```

Compile:

```bash
gcc printf.c -o printf
./printf
```

It can also be useful to enable warnings:

```bash
gcc -Wall -Wextra printf.c -o printf
```

> On a modern compiler, `%O` and `%U` are not standard C `printf()` conversion specifiers. Standard octal and unsigned-decimal conversions are lowercase `%o` and `%u`. This makes a good opportunity to look at compiler warnings.

---

## Integer formats

```c
%d
%i
%x
%X
%o
%u
```

| Specifier | Meaning |
|---|---|
| `%d` | signed decimal integer |
| `%i` | signed decimal integer |
| `%x` | hexadecimal, lowercase |
| `%X` | hexadecimal, uppercase |
| `%o` | octal |
| `%u` | unsigned decimal |

Try changing:

```c
int a1 = 23;
```

to another value and predict its hexadecimal and octal representations.

---

## Number literals

These assignments are especially useful:

```c
a1 = 023;
b1 = 0x53;
```

A leading `0` means the integer literal is written in **octal**.

So:

```c
023
```

means:

```text
2 × 8 + 3 = 19
```

A leading `0x` means **hexadecimal**.

So:

```c
0x53
```

means:

```text
5 × 16 + 3 = 83
```

---

## Floating-point formats

```c
%f
%e
%E
%g
%G
```

Try the same value using each format.

For example:

```c
printf("%f\n", 12345.678);
printf("%e\n", 12345.678);
printf("%g\n", 12345.678);
```

Ask:

> Which representation is easiest to read?

---

## Characters are numbers too

```c
printf("%c, %c, %c\n", '&', 0x4B, 87);
```

`%c` interprets the integer value as a character code.

Predict what:

```c
0x4B
```

and:

```c
87
```

will print before running.

---

## The null character `\0`

Look at:

```c
printf("%s%s\n", "Hello \n World\0 Welcome!", "!");
```

A C string ends at:

```c
'\0'
```

Therefore `printf("%s", ...)` stops printing when it reaches that null terminator.

Question:

> Will `Welcome!` appear?

Predict first, then run.

---

# Field Width

Example:

```c
printf("%12d\n", 23);
```

The `12` specifies a minimum field width.

This is useful for aligning columns.

Try:

```c
printf("%10d\n", 1);
printf("%10d\n", 22);
printf("%10d\n", 333);
```

Then try left alignment:

```c
printf("%-10dX\n", 333);
```

---

# Zero Padding

```c
printf("%012d\n", 23);
```

Try to predict how many zeros appear.

This idea is useful for things like IDs:

```c
printf("%09d\n", 1234);
```

which produces a 9-character, zero-padded field.

---

# Precision

```c
printf("%.2f\n", 3.14159);
```

prints two digits after the decimal point.

Try:

```c
printf("%.0f\n", 3.14159);
printf("%.1f\n", 3.14159);
printf("%.2f\n", 3.14159);
printf("%.3f\n", 3.14159);
```

Notice that formatting performs **rounding**.

---

# Dynamic Width and Precision

```c
printf("%*.*f\n", 5, 3, 3.14159);
```

The `*` values are supplied as arguments.

Here:

```text
width     = 5
precision = 3
value     = 3.14159
```

Try changing the first two numbers.

---

## `printf.c` challenge

Without running it first, write a `printf()` statement that prints:

```text
ID:    000123
Score:  93.46
```

Then test your answer.

---

# 5. `loop.c` — `while`, `do-while`, and `for`

## Code

```c
/* This program reads a student ID (int), a score (float), and assigns
        a grade (char) based on the score and prints them out. It then
        asks the same information for the next student till a given
        number of student's data in entered.
   Features in this program:
   1. while
   2. do-while
   3. for

                                                        Gang Qu
                                                        Sept. 1, 2015
*/

#include <stdio.h>

int main(void)
{
    int SID;
    float Score;
    char Grade;
    int No = 1;

    /* while loop */
    while (No <= 3)
    {
        printf("Enter the student ID and score:");
        scanf("%d %f", &SID, &Score);

        if (Score >= 60.0)
        {
            if (Score >= 90.0)
                Grade = 'A';
            else
                Grade = 'P';
        }
        else
        {
            Grade = 'F';
        }

        printf("SID: %d\nScore: %f\nGrade: %c\n",
               SID, Score, Grade);

        No = No + 1;
    }

    /*
    do
    {
        printf("Enter the student ID and score:");
        scanf("%d %f", &SID, &Score);

        if (Score >= 60.0)
        {
            if (Score >= 90.0)
                Grade = 'A';
            else
                Grade = 'P';
        }
        else
        {
            Grade = 'F';
        }

        printf("SID: %d\nScore: %f\nGrade: %c\n",
               SID, Score, Grade);

        No = No + 1;
    } while (No <= 3);
    */

    /*
    for (No = 1; No <= 3; No = No + 1)
    {
        printf("Enter the student ID and score:");
        scanf("%d %f", &SID, &Score);

        if (Score >= 60.0)
        {
            if (Score >= 90.0)
                Grade = 'A';
            else
                Grade = 'P';
        }
        else
        {
            Grade = 'F';
        }

        printf("SID: %d\nScore: %f\nGrade: %c\n",
               SID, Score, Grade);
    }
    */

    return 0;
}
```

Compile:

```bash
gcc loop.c -o loop
./loop
```

---

# The three loops do the same repeated job

Each implementation is intended to process three students.

The important loop variable is:

```c
No
```

which stands for **number**.

---

# A. `while` loop

```c
No = 1;

while (No <= 3)
{
    ...
    No = No + 1;
}
```

## Flow

```text
initialize No
     |
     v
check condition ---- false ----> leave loop
     |
    true
     |
     v
 execute body
     |
     v
increment No
     |
     +-----------> check condition again
```

## Trace `No`

| Time | `No` |
|---|---:|
| Before loop | 1 |
| Iteration 1 begins | 1 |
| Iteration 1 ends | 2 |
| Iteration 2 begins | 2 |
| Iteration 2 ends | 3 |
| Iteration 3 begins | 3 |
| Iteration 3 ends | 4 |
| Check `4 <= 3` | false |

The loop body executes **three times**.

---

# B. `do-while` loop

```c
do
{
    ...
    No = No + 1;
} while (No <= 3);
```

The major difference is **when the condition is checked**.

A `while` loop checks before entering.

A `do-while` loop checks **after** executing the body.

Therefore a `do-while` loop always executes at least once.

## Thought experiment

Suppose:

```c
No = 100;
```

What happens?

### `while`

```c
while (No <= 3)
```

is immediately false, so the body executes **zero times**.

### `do-while`

The body executes once first, and only then checks the condition.

So it executes **once**.

---

# C. `for` loop

```c
for (No = 1; No <= 3; No = No + 1)
```

A `for` loop packages three pieces together:

```c
for ( initialization ; condition ; update )
```

Here:

```c
No = 1
```

is initialization,

```c
No <= 3
```

is the condition,

and:

```c
No = No + 1
```

is the update.

Equivalent idea:

```c
No = 1;

while (No <= 3)
{
    ...
    No = No + 1;
}
```

---

## Important when demonstrating the alternatives

The source contains the three loop implementations as alternatives.

If you enable a different version, make sure `No` starts at the intended value.

For example, after the original `while` finishes:

```text
No = 4
```

If you immediately execute the `do-while` afterward without resetting `No`, the behavior will be different.

For a clean comparison, run **one implementation at a time** or reset:

```c
No = 1;
```

before the next version.

---

# Grade logic inside the loop

```c
if (Score >= 60.0)
{
    if (Score >= 90.0)
        Grade = 'A';
    else
        Grade = 'P';
}
else
{
    Grade = 'F';
}
```

Trace three examples.

### Score = 95

```text
95 >= 60 ? true
95 >= 90 ? true
Grade = A
```

### Score = 75

```text
75 >= 60 ? true
75 >= 90 ? false
Grade = P
```

### Score = 40

```text
40 >= 60 ? false
Grade = F
```

---

# 6. `ifelse.c` — Different Ways to Implement the Same Selection

For the classroom exercise, change:

```c
#define ROSTER 59
```

to:

```c
#define ROSTER 5
```

This lets you enter five test scores without entering 59 students.

Suggested test scores:

```text
95.2
83
76
60.5
49
```

These produce:

| Score | Grade |
|---:|:---:|
| 95.2 | A |
| 83 | B |
| 76 | C |
| 60.5 | D |
| 49 | F |

---

## Relevant definitions

```c
#define ROSTER 5
#define LowA 90.0
#define LowB 80.0
#define LowC 70.0
#define LowD 60.0
```

The program eventually places a score into:

```c
s
```

and computes a grade in:

```c
g
```

---

# Implementation 1 — Nested descending `if` / `else`

```c
if (s >= LowA)
    g = 'A';
else
    if (s >= LowB)
        g = 'B';
    else
        if (s >= LowC)
            g = 'C';
        else
            if (s >= LowD)
                g = 'D';
            else
                g = 'F';
```

The important property is:

> As soon as one condition is true, the remaining tests are skipped.

---

## Trace `s = 83`

Start:

```c
if (83 >= 90)
```

False.

Go to the associated `else`:

```c
if (83 >= 80)
```

True.

So:

```c
g = 'B';
```

and the rest is skipped.

Comparisons:

```text
83 >= 90
83 >= 80
```

Total: **2**

---

## Number of comparisons

| Score | Comparisons |
|---:|---:|
| 95.2 | 1 |
| 83 | 2 |
| 76 | 3 |
| 60.5 | 4 |
| 49 | 4 |

---

# Implementation 2 — Nested `<` tests

```c
if (s < LowA)
    if (s < LowB)
        if (s < LowC)
            if (s < LowD)
                g = 'F';
            else
                g = 'D';
        else
            g = 'C';
    else
        g = 'B';
else
    g = 'A';
```

This works from the opposite direction.

---

## Trace `s = 76`

```text
76 < 90 ? true
    |
76 < 80 ? true
    |
76 < 70 ? false
    |
    +----> g = 'C'
```

Total comparisons: **3**

---

## Number of comparisons

| Score | Comparisons |
|---:|---:|
| 95.2 | 1 |
| 83 | 2 |
| 76 | 3 |
| 60.5 | 4 |
| 49 | 4 |

For these thresholds, Implementations 1 and 2 have the same comparison counts for the selected representative grades.

---

# Implementation 3 — Separate independent `if` statements

```c
if (s >= LowA)
    g = 'A';

if (s >= LowB && s < LowA)
    g = 'B';

if (s >= LowC && s < LowB)
    g = 'C';

if (s >= LowD && s < LowC)
    g = 'D';

if (s < LowD)
    g = 'F';
```

This is fundamentally different.

There is no `else` connecting the statements.

Therefore **every `if` statement is reached**.

Even after the program discovers that a score is an A, it continues testing the B, C, D, and F conditions.

---

## Short-circuit evaluation

Consider:

```c
s >= LowB && s < LowA
```

For `&&`, C evaluates left to right.

If:

```c
s >= LowB
```

is false, the whole expression must be false, so:

```c
s < LowA
```

does not need to be evaluated.

This is called **short-circuit evaluation**.

---

## Primitive score comparisons

Counting each relational operation such as:

```c
s >= LowB
```

or:

```c
s < LowA
```

as one comparison:

| Score | Approx. relational comparisons |
|---:|---:|
| 95.2 | 8 |
| 83 | 8 |
| 76 | 7 |
| 60.5 | 6 |
| 49 | 5 |

This implementation generally performs more work because the grade ranges are checked independently.

---

# Implementation 4 — Split near the middle first

```c
if (s >= LowB)
    if (s >= LowA)
        g = 'A';
    else
        g = 'B';
else if (s >= LowC)
    g = 'C';
else if (s >= LowD)
    g = 'D';
else
    g = 'F';
```

This version first asks:

```text
Is the score at least 80?
```

That divides the possibilities roughly into:

```text
A/B
```

versus:

```text
C/D/F
```

---

## Trace `s = 95.2`

```text
95.2 >= 80 ? true
95.2 >= 90 ? true
g = A
```

Total: **2 comparisons**

---

## Trace `s = 60.5`

```text
60.5 >= 80 ? false
60.5 >= 70 ? false
60.5 >= 60 ? true
g = D
```

Total: **3 comparisons**

---

## Comparison count

| Score | Comparisons |
|---:|---:|
| 95.2 | 2 |
| 83 | 2 |
| 76 | 2 |
| 60.5 | 3 |
| 49 | 3 |

This can use fewer comparisons for scores in the middle of the range than a simple top-down chain.

---

# Implementation 5 — `?:`

The source also contains:

```c
g = (s<LowA)?(s<LowB)?(s<LowC)?(s<LowD)?'F':'D':'C':'B':'A';
```

For this walkthrough, **skip this implementation** as requested.

The `?:` operator is the conditional/ternary operator and will be easier to understand after students are comfortable with ordinary `if` statements.

---

# Implementation 6 — `switch`

```c
switch ((int)s / 10)
{
    case 10:
    case 9:
        g = 'A';
        break;

    case 8:
        g = 'B';
        break;

    case 7:
        g = 'C';
        break;

    case 6:
        g = 'D';
        break;

    default:
        g = 'F';
}
```

First:

```c
(int)s
```

converts the score to an integer.

Then:

```c
(int)s / 10
```

groups scores by their tens digit.

Examples:

```text
95.2 -> 95 -> 9
83   -> 83 -> 8
76   -> 76 -> 7
60.5 -> 60 -> 6
49   -> 49 -> 4
```

Then the `switch` selects the matching case.

| Score | `(int)s / 10` | Case | Grade |
|---:|---:|---:|:---:|
| 95.2 | 9 | `case 9` | A |
| 83 | 8 | `case 8` | B |
| 76 | 7 | `case 7` | C |
| 60.5 | 6 | `case 6` | D |
| 49 | 4 | `default` | F |

There are no explicit score-vs-threshold relational comparisons in this implementation. How the compiler implements the `switch` internally is compiler-dependent, so do not assign it a fixed low-level comparison count.

---

# Comparing the `ifelse.c` implementations

For the sample values:

| Score | Grade | Impl. 1 | Impl. 2 | Impl. 3* | Impl. 4 |
|---:|:---:|---:|---:|---:|---:|
| 95.2 | A | 1 | 1 | 8 | 2 |
| 83 | B | 2 | 2 | 8 | 2 |
| 76 | C | 3 | 3 | 7 | 2 |
| 60.5 | D | 4 | 4 | 6 | 3 |
| 49 | F | 4 | 4 | 5 | 3 |

\* Implementation 3 counts each primitive relational operation and accounts for `&&` short-circuiting.

The important lesson is not simply:

> fewer comparisons = always better code

Instead, ask:

- Is the control flow easy to understand?
- Are unnecessary conditions evaluated?
- Does the ordering make sense for the expected data?
- Can a future programmer modify the grading thresholds safely?
- Do two implementations produce the same result for all valid scores?

---

# Interactive `ifelse.c` walkthrough

Use one score and physically trace the lines.

Example:

```text
s = 76
```

For Implementation 1:

```text
s >= 90 ?  FALSE
              |
              v
s >= 80 ?  FALSE
              |
              v
s >= 70 ?  TRUE
              |
              v
           g = 'C'
```

Now ask students to trace:

```text
95.2
83
60.5
49
```

before running the program.

---

# 7. Compile / Run Checklist

For any file:

```bash
gcc filename.c -o program_name
```

Example:

```bash
gcc loop.c -o loop
```

Then run:

```bash
./loop
```

For warnings:

```bash
gcc -Wall -Wextra loop.c -o loop
```

A strong habit is:

> **Read compiler warnings instead of ignoring them.**

---

# 8. Homework 1 Submission Reminder — `cp`

Homework 1 files should be named exactly:

```text
hw1_x.c
```

where `x` is the problem number.

Examples:

```text
hw1_1.c
hw1_2.c
hw1_3.c
```

Use the class's provided `cp` command to copy the file into the Homework 1 submission directory.

If you later update a file, run the **same `cp` command again**.

For example, when re-submitting:

```text
hw1_3.c
```

you may see something like:

```text
overwrite ‘..../hw1_3.c’?
```

Type:

```text
y
```

and press Enter.

## Important

When you overwrite the file, the **previous submitted copy is lost** and replaced by the new one.

Before typing `y`, make sure the updated file is really the version you want to submit.

---

# 9. Suggested Teaching Pattern

For each program, use this cycle:

### 1. Predict

Do **not** compile immediately.

Ask:

> What do you think will happen?

### 2. Trace

Write down the important variables.

Examples:

```text
a =
i =
No =
s =
g =
```

### 3. Identify control flow

At every condition, ask:

```text
true or false?
```

Then explicitly identify which line executes next.

### 4. Compile

```bash
gcc -Wall -Wextra file.c -o file
```

### 5. Run

```bash
./file
```

### 6. Compare

Ask:

> Was our prediction correct?

If not:

> Which assumption was wrong?

That last question is usually where the most useful learning happens.

---

# Quick Reference

## `scanf()`

```c
scanf("%d", &integer);
scanf("%f", &floating_point);
scanf(" %c", &character);
scanf("%s", string);
```

## `printf()`

```c
printf("%d", integer);
printf("%f", floating_point);
printf("%c", character);
printf("%s", string);
```

## `while`

```c
while (condition)
{
    statements;
}
```

## `do-while`

```c
do
{
    statements;
} while (condition);
```

## `for`

```c
for (initialization; condition; update)
{
    statements;
}
```

## `if / else`

```c
if (condition)
{
    statements;
}
else
{
    statements;
}
```

## `switch`

```c
switch (expression)
{
    case value:
        statements;
        break;

    default:
        statements;
}
```

---

# Final Questions for Students

After working through these examples, you should be able to answer:

1. What is the difference between a global variable and a local variable?
2. What does `static` do to a local variable?
3. What happens when a local variable has the same name as a global variable?
4. Why does `scanf()` usually need `&`?
5. Why can `%c` behave differently from `%d` or `%f` when whitespace is present?
6. What is the difference between a `while` and a `do-while` loop?
7. How can a `for` loop be rewritten as a `while` loop?
8. How does an `if / else if / else` chain prevent unnecessary tests?
9. What is short-circuit evaluation?
10. How does the `switch` implementation turn scores into grade categories?
11. What is the difference between `++i` and `i++` when their value is used in an expression?
12. What happens to the old submitted homework file when you answer `y` to an overwrite prompt?
