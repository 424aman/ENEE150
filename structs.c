#include <stdio.h>
#include <string.h>

// --------------------------------------------------
// 1️⃣ Plain struct — must use "struct" keyword every time
// --------------------------------------------------
struct Point {
    int x;
    int y;
};

// --------------------------------------------------
// 2️⃣ Typedef struct — creates a cleaner alias (no "struct" keyword needed)
// --------------------------------------------------
typedef struct {
    char name[30];
    int age;
} Person;

// --------------------------------------------------
// 3️⃣ Nested struct — defines a struct inside another
// --------------------------------------------------
typedef struct {
    char title[50];
    int pages;

    // nested struct defined INSIDE
    struct {
        int day;
        int month;
        int year;
    } published;

    // also embedding a typedef struct
    Person author;
} Book;

// --------------------------------------------------
// Main demo
// --------------------------------------------------
int main(void) {
    printf("=== STRUCT DEMO ===\n\n");

    // ----- Plain struct -----
    struct Point p1 = {3, 4};
    printf("Point: (%d, %d)\n", p1.x, p1.y);

    // You must include "struct" every time for plain structs:
    struct Point p2;
    p2.x = 10;
    p2.y = 20;
    printf("Another Point: (%d, %d)\n\n", p2.x, p2.y);

    // ----- Typedef struct -----
    Person person1 = {"Aman Garg", 20};
    printf("Person: %s, age %d\n", person1.name, person1.age);

    // Using pointers to typedef struct
    Person *ptr = &person1;
    ptr->age = 21; // arrow operator
    printf("Updated age (via pointer): %d\n\n", ptr->age);

    // ----- Nested struct -----
    Book book1;

    strcpy(book1.title, "Learning C");
    book1.pages = 250;
    book1.published.day = 17;
    book1.published.month = 10;
    book1.published.year = 2025;

    // Set nested typedef struct inside Book
    strcpy(book1.author.name, "Brian Kernighan");
    book1.author.age = 82;

    // Access nested struct fields
    printf("Book: %s (%d pages)\n", book1.title, book1.pages);
    printf("Published on: %02d/%02d/%04d\n",
           book1.published.day,
           book1.published.month,
           book1.published.year);
    printf("Author: %s, age %d\n\n", book1.author.name, book1.author.age);

    // Copying structs directly
    Book copy = book1;
    strcpy(copy.title, "The C Bible");
    copy.published.year = 2026;

    printf("Copy Book: %s (original was '%s')\n",
           copy.title, book1.title);

    return 0;
}

