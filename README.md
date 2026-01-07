# using_STM32F446re
Meant to be used to practice driver developtment.

## What will you find in this repo.

  1). driver_developtment_NO_IDE:

    Provides the user with all the files in order to be able to use the STM32F446re without the need of a particular IDE.

  2). driver_developtment_Uvision_Keil:

    Uses Keil uVision. Keil uVision is a comprehensive software development environment, primarily used for developing embedded applications, especially for Arm-based microcontrollers. It's essentially an IDE (Integrated Development Environment) that includes tools for project management, code editing, compilation, debugging, and simulation.

# C99 vs POSIX — Simple Function Reference

This document is a **plain, practical reference** that answers one question:

> **Is the function I want part of C99 or POSIX, and where does it live?**

No theory. No diagrams. No cross-linking.

---

# C99 STANDARD LIBRARY
(ISO/IEC 9899:1999)

✔ Portable  
✔ Works in hosted and freestanding C  
✔ Used in embedded, RTOS, Linux, bare metal (subset)

---

## Memory & Allocation — <stdlib.h>
- malloc
- calloc
- realloc
- free

---

## Strings & Memory — <string.h>
- memcpy
- memmove
- memset
- memcmp
- strlen
- strcpy
- strncpy
- strcmp
- strncmp
- strcat
- strncat
- strchr
- strstr
- strtok
- strerror

---

## Input / Output — <stdio.h>
- printf
- fprintf
- sprintf
- snprintf
- scanf
- fopen
- fclose
- fread
- fwrite
- fgets
- fputs
- fflush
- perror

---

## Character Handling — <ctype.h>
- isalpha
- isdigit
- isalnum
- isspace
- isupper
- islower
- tolower
- toupper

---

## Math — <math.h>
- sqrt
- pow
- sin
- cos
- tan
- exp
- log
- floor
- ceil
- fabs

---

## Time & Date — <time.h>
- time
- clock
- difftime
- gmtime
- localtime
- strftime

---

## Program Control — <stdlib.h>
- exit
- abort
- atexit
- system

---

## Errors — <errno.h>
- errno
- EINVAL
- ENOENT
- EACCES
- EINTR

---

## Assertions — <assert.h>
- assert

---

## Variadic Arguments — <stdarg.h>
- va_start
- va_arg
- va_end
- va_copy

---

## Fixed-Width Integers — <stdint.h>
- int8_t, int16_t, int32_t, int64_t
- uint8_t, uint16_t, uint32_t, uint64_t

(No functions)

---

## Boolean Type — <stdbool.h>
- bool
- true
- false

(No functions)

---

# POSIX API
(IEEE 1003.x)

❌ Not available in bare-metal C  
✔ Available on Linux, BSD, macOS  
✔ Partial support in RTOS

---

## Processes — <unistd.h>
- fork
- execv
- execvp
- execl
- execve
- _exit
- getpid
- getppid

---

## Process Status — <sys/wait.h>
- wait
- waitpid
- waitid

Macros:
- WIFEXITED
- WEXITSTATUS
- WIFSIGNALED
- WTERMSIG

---

## Files (File Descriptors) — <unistd.h>, <fcntl.h>
- open
- openat
- read
- write
- close
- lseek
- dup
- dup2

---

## File Metadata & Permissions — <sys/stat.h>
- stat
- fstat
- lstat
- mkdir
- chmod
- umask

---

## Directories — <dirent.h>
- opendir
- readdir
- closedir
- rewinddir

---

## Signals — <signal.h>
- signal
- sigaction
- kill
- sigemptyset
- sigaddset
- sigprocmask

Signals:
- SIGINT
- SIGTERM
- SIGKILL
- SIGCHLD
- SIGSEGV

---

## Threads — <pthread.h>
- pthread_create
- pthread_join
- pthread_exit
- pthread_cancel
- pthread_self

Mutexes:
- pthread_mutex_init
- pthread_mutex_lock
- pthread_mutex_unlock

---

## Semaphores — <semaphore.h>
- sem_init
- sem_wait
- sem_post
- sem_destroy

---

## Memory Mapping — <sys/mman.h>
- mmap
- munmap
- mprotect
- msync

---

## IPC — Pipes & Multiplexing
### <unistd.h>
- pipe

### <poll.h>
- poll

### <sys/select.h>
- select
- pselect

---

## Networking — <sys/socket.h>
- socket
- bind
- listen
- accept
- connect
- send
- recv

---

## Internet Addresses — <netinet/in.h>, <arpa/inet.h>
- htons
- htonl
- ntohs
- ntohl
- inet_pton
- inet_ntop

---

## Time (POSIX) — <sys/time.h>
- gettimeofday
- settimeofday
- timersub

---

## Resource Limits — <sys/resource.h>
- getrlimit
- setrlimit

---

# RULES OF THUMB

- If it touches **memory allocation** → C99
- If it touches **strings or math** → C99
- If it touches **processes** → POSIX
- If it touches **file descriptors** → POSIX
- If it touches **threads or signals** → POSIX
- If it touches **hardware registers** → neither

---

# FINAL SUMMARY

- **C99** → Language + runtime utilities
- **POSIX** → Operating system services

If you are on **Linux**, you use both.
If you are on **bare metal**, you use C99 only (subset).

