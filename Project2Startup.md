# ENEE150 Project 2 — Email Viewer

## Overview
This program parses an email database file and lets the user:
1. Print a summary of all emails.
2. Print a specific email message.
3. Search emails by keyword.

Each email in the database has:
- A control-L delimiter (ASCII 0x0C or 12).
- Three header lines: From, Subject, Date.
- A body starting after "Message:\n" until the next control-L.

You must:
- Parse headers, extract fields.
- Convert dates/times
- Store all emails, then allow user menu navigation.

---

## Suggested Memory Approach
The file size never exceeds **10 MB** and at most 1000 emails.
One good approach is:
1. Read the entire file into memory.
2. Parse emails directly within that memory (using pointers).
3. Build an array of `struct Email` objects, each storing pointers into this buffer.

This avoids costly reallocations and allows fast substring parsing.

---

## Functions you may find helpful
1. strstr(const char *haystack, const char *needle)
   → Finds first occurrence of substring.
   Example:
       if (strstr(line, "Subject:")) { ... }

2. strchr(const char *s, int c)
   → Finds first occurrence of a single character.
   Example:
       char *at = strchr(email, '@');

3. strrchr(const char *s, int c)
   → Finds the last occurrence of a character.
   Example:
       char *dot = strrchr(filename, '.');

4. memchr(const void *ptr, int c, size_t n)
   → Like strchr but works on raw memory (useful if '\0' may appear early).

5. strtok(char *str, const char *delim)
   → Splits a string into tokens separated by delimiters.
   Example:
       char *token = strtok(line, " ");
       while (token) {
           printf("%s\n", token);
           token = strtok(NULL, " ");
       }

7. strlen(const char *s)
   → Returns string length (excluding '\0').

8. strcpy(char *dest, const char *src)
   → Copies entire string. Be careful with buffer sizes!

9. strncpy(char *dest, const char *src, size_t n)
   → Safe copy, but ensure dest[n-1] = '\0' manually.

10. strcat / strncat
   → Appends one string to another.

11. strcmp(const char *s1, const char *s2)
   → Returns 0 if strings are equal.

13. isspace(int c)
   → Checks for whitespace characters.
