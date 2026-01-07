# using_STM32F446re
Meant to be used to practice driver developtment.

## What will you find in this repo.

  1). driver_developtment_NO_IDE:

    Provides the user with all the files in order to be able to use the STM32F446re without the need of a particular IDE.

  2). driver_developtment_Uvision_Keil:

    Uses Keil uVision. Keil uVision is a comprehensive software development environment, primarily used for developing embedded applications, especially for Arm-based microcontrollers. It's essentially an IDE (Integrated Development Environment) that includes tools for project management, code editing, compilation, debugging, and simulation.

/*
=========================================================
C99 vs POSIX — Simple Function Reference (Fully Annotated)
=========================================================

Purpose:
Identify whether a function belongs to C99 or POSIX,
what it does, and how to interpret its return value.

All explanations are comments.
All prototypes are real.
=========================================================
*/

/* ======================================================
   C99 STANDARD LIBRARY (ISO/IEC 9899:1999)
   ====================================================== */

/* ---------------- Memory & Allocation — <stdlib.h> ---------------- */

void *malloc(size_t size);
/*
Allocates `size` bytes of uninitialized memory.
Success: returns pointer to allocated memory.
Failure: returns NULL.
*/

void *calloc(size_t nmemb, size_t size);
/*
Allocates memory for an array and zero-initializes it.
Success: returns pointer to allocated memory.
Failure: returns NULL.
*/

void *realloc(void *ptr, size_t size);
/*
Resizes previously allocated memory block.
Success: returns pointer to resized block.
Failure: returns NULL, original block remains valid.
*/

void free(void *ptr);
/*
Frees memory previously allocated.
Success: memory released.
Failure: undefined behavior if ptr is invalid.
Returns nothing.
*/

/* ---------------- Strings & Memory — <string.h> ---------------- */

void *memcpy(void *dest, const void *src, size_t n);
/*
Copies `n` bytes from src to dest (overlap NOT allowed).
Success: returns dest.
Failure: undefined behavior on overlap.
*/

void *memmove(void *dest, const void *src, size_t n);
/*
Copies `n` bytes from src to dest (overlap-safe).
Success: returns dest.
Failure: none defined.
*/

void *memset(void *s, int c, size_t n);
/*
Sets `n` bytes at s to value c.
Success: returns s.
*/

int memcmp(const void *s1, const void *s2, size_t n);
/*
Compares `n` bytes.
Returns <0 if s1 < s2,
        0 if equal,
       >0 if s1 > s2.
*/

size_t strlen(const char *s);
/*
Computes length of string excluding null terminator.
Returns number of characters.
*/

char *strcpy(char *dest, const char *src);
/*
Copies src string into dest (no bounds checking).
Success: returns dest.
Failure: undefined behavior if dest too small.
*/

char *strncpy(char *dest, const char *src, size_t n);
/*
Copies up to n characters.
May not null-terminate.
Returns dest.
*/

int strcmp(const char *s1, const char *s2);
/*
Lexicographically compares strings.
Returns <0, 0, or >0.
*/

int strncmp(const char *s1, const char *s2, size_t n);
/*
Compares up to n characters.
Returns <0, 0, or >0.
*/

char *strcat(char *dest, const char *src);
/*
Appends src to dest.
Returns dest.
Undefined behavior if dest too small.
*/

char *strncat(char *dest, const char *src, size_t n);
/*
Appends up to n characters.
Returns dest.
*/

char *strchr(const char *s, int c);
/*
Finds first occurrence of character c.
Success: returns pointer to character.
Failure: returns NULL.
*/

char *strstr(const char *haystack, const char *needle);
/*
Finds first occurrence of substring.
Success: returns pointer.
Failure: returns NULL.
*/

char *strtok(char *str, const char *delim);
/*
Tokenizes string using delimiters.
Success: returns pointer to token.
Failure: returns NULL.
Not thread-safe.
*/

char *strerror(int errnum);
/*
Maps errno value to error string.
Returns pointer to static string.
*/

/* ---------------- Input / Output — <stdio.h> ---------------- */

int printf(const char *format, ...);
/*
Writes formatted output to stdout.
Success: returns number of characters printed.
Failure: returns negative value.
*/

int fprintf(FILE *stream, const char *format, ...);
/*
Writes formatted output to stream.
Returns characters printed or negative on error.
*/

int sprintf(char *str, const char *format, ...);
/*
Writes formatted output to string.
Returns number of characters written.
No bounds checking.
*/

int snprintf(char *str, size_t size, const char *format, ...);
/*
Writes formatted output with size limit.
Returns number of characters that would have been written.
*/

int scanf(const char *format, ...);
/*
Reads formatted input from stdin.
Returns number of items assigned or EOF on failure.
*/

FILE *fopen(const char *path, const char *mode);
/*
Opens file stream.
Success: returns FILE pointer.
Failure: returns NULL.
*/

int fclose(FILE *stream);
/*
Closes file stream.
Success: returns 0.
Failure: returns EOF.
*/

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
/*
Reads data from stream.
Returns number of elements read.
*/

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
/*
Writes data to stream.
Returns number of elements written.
*/

char *fgets(char *s, int size, FILE *stream);
/*
Reads a line from stream.
Success: returns s.
Failure or EOF: returns NULL.
*/

int fputs(const char *s, FILE *stream);
/*
Writes string to stream.
Success: non-negative value.
Failure: EOF.
*/

int fflush(FILE *stream);
/*
Flushes output buffer.
Success: returns 0.
Failure: EOF.
*/

void perror(const char *s);
/*
Prints descriptive error message for errno.
Returns nothing.
*/

/* ---------------- Character Handling — <ctype.h> ---------------- */

int isalpha(int c);   /* returns nonzero if alphabetic, 0 otherwise */
int isdigit(int c);   /* returns nonzero if digit */
int isalnum(int c);   /* returns nonzero if alphanumeric */
int isspace(int c);   /* returns nonzero if whitespace */
int isupper(int c);   /* returns nonzero if uppercase */
int islower(int c);   /* returns nonzero if lowercase */
int tolower(int c);   /* converts to lowercase, returns result */
int toupper(int c);   /* converts to uppercase, returns result */

/* ---------------- Math — <math.h> ---------------- */

double sqrt(double x);    /* returns square root or NaN */
double pow(double x, double y);
double sin(double x);
double cos(double x);
double tan(double x);
double exp(double x);
double log(double x);
double floor(double x);
double ceil(double x);
double fabs(double x);

/* ======================================================
   POSIX API (IEEE 1003.x)
   ====================================================== */

/* ---------------- Processes — <unistd.h> ---------------- */

pid_t fork(void);
/*
Creates a new process.
Parent: returns child PID.
Child: returns 0.
Failure: returns -1.
*/

int execv(const char *path, char *const argv[]);
/*
Replaces current process image.
Success: does NOT return.
Failure: returns -1 and sets errno.
*/

int execvp(const char *file, char *const argv[]);
/*
Executes program using PATH.
Success: does NOT return.
Failure: returns -1.
*/

void _exit(int status);
/*
Terminates process immediately.
Does not flush stdio buffers.
Does not return.
*/

pid_t getpid(void);   /* returns current process ID */
pid_t getppid(void);  /* returns parent process ID */

/* ---------------- Process Status — <sys/wait.h> ---------------- */

pid_t waitpid(pid_t pid, int *status, int options);
/*
Waits for child process.
Success: returns PID.
Failure: returns -1.
*/

int WIFEXITED(int status);    /* nonzero if child exited normally */
int WEXITSTATUS(int status); /* extracts exit code */

/* ---------------- Files — <unistd.h>, <fcntl.h> ---------------- */

int open(const char *path, int flags, ...);
/*
Opens file.
Success: returns file descriptor.
Failure: returns -1.
*/

ssize_t read(int fd, void *buf, size_t count);
/*
Reads from file descriptor.
Success: returns bytes read.
EOF: returns 0.
Failure: returns -1.
*/

ssize_t write(int fd, const void *buf, size_t count);
/*
Writes to file descriptor.
Success: returns bytes written.
Failure: returns -1.
*/

int close(int fd);
/*
Closes file descriptor.
Success: returns 0.
Failure: returns -1.
*/

/* ======================================================
   END OF REFERENCE
   ====================================================== */
