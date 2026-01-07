# using_STM32F446re
Meant to be used to practice driver developtment.

## What will you find in this repo.

  1). driver_developtment_NO_IDE:

    Provides the user with all the files in order to be able to use the STM32F446re without the need of a particular IDE.

  2). driver_developtment_Uvision_Keil:

    Uses Keil uVision. Keil uVision is a comprehensive software development environment, primarily used for developing embedded applications, especially for Arm-based microcontrollers. It's essentially an IDE (Integrated Development Environment) that includes tools for project management, code editing, compilation, debugging, and simulation.
/*
===========================================================
C99 + POSIX — Quick Semantics (what it does / success / fail)
===========================================================
*/
```c
/* ======================= C99 ======================= */

/* <stdlib.h> */
void *malloc(size_t size);
/* Does: allocate `size` bytes (uninitialized).
   Success: returns pointer to block.
   Fail: returns NULL. */

void *calloc(size_t nmemb, size_t size);
/* Does: allocate array nmemb*size and zero it.
   Success: returns pointer.
   Fail: returns NULL. */

void *realloc(void *ptr, size_t size);
/* Does: resize previously allocated block.
   Success: returns pointer to resized (maybe moved) block.
   Fail: returns NULL and original `ptr` remains valid (unchanged). */

void free(void *ptr);
/* Does: release heap memory from malloc/calloc/realloc.
   Success: no return (memory released).
   Fail: no defined “failure”; passing invalid ptr => undefined behavior. */

void exit(int status);
/* Does: terminate program normally (flushes stdio, runs atexit handlers).
   Success: does not return. */

void abort(void);
/* Does: abnormal termination (usually raises SIGABRT).
   Success: does not return. */

int atexit(void (*func)(void));
/* Does: register function to run at normal exit().
   Success: returns 0.
   Fail: returns nonzero. */

int system(const char *command);
/* Does: run `command` via shell.
   Success: returns a wait-status (use WIFEXITED/WEXITSTATUS) or nonzero if command==NULL and shell exists.
   Fail: returns -1 and sets errno (e.g., fork failure). */

long strtol(const char *nptr, char **endptr, int base);
/* Does: convert string to long.
   Success: returns converted value; endptr points to first non-parsed char.
   Fail: on overflow/underflow sets errno=ERANGE; if no digits parsed, endptr==nptr (errno may be unchanged). */

double strtod(const char *nptr, char **endptr);
/* Does: convert string to double.
   Success: returns converted value; endptr advanced.
   Fail: on overflow sets errno=ERANGE; if no conversion, endptr==nptr. */

/* <string.h> */
void *memcpy(void *dest, const void *src, size_t n);
/* Does: copy n bytes src->dest (overlap NOT allowed).
   Success: returns dest.
   Fail: no error return; overlapping regions => undefined behavior. */

void *memmove(void *dest, const void *src, size_t n);
/* Does: copy n bytes src->dest (overlap allowed).
   Success: returns dest.
   Fail: no defined failure. */

void *memset(void *s, int c, size_t n);
/* Does: set n bytes at s to byte value (unsigned char)c.
   Success: returns s.
   Fail: no defined failure. */

int memcmp(const void *s1, const void *s2, size_t n);
/* Does: compare n bytes.
   Success: returns <0, 0, >0 based on lexicographic byte comparison.
   Fail: no defined failure. */

size_t strlen(const char *s);
/* Does: count chars until '\0'.
   Success: returns length (not including '\0').
   Fail: if s not NUL-terminated/invalid => undefined behavior. */

char *strcpy(char *dest, const char *src);
/* Does: copy src string including terminating '\0'.
   Success: returns dest.
   Fail: if dest too small/overlap invalid => undefined behavior. */

char *strncpy(char *dest, const char *src, size_t n);
/* Does: copy up to n chars; pads with '\0' if src shorter; may NOT NUL-terminate if src length >= n.
   Success: returns dest.
   Fail: misuse can cause non-terminated strings; invalid pointers => undefined behavior. */

int strcmp(const char *s1, const char *s2);
/* Does: compare strings.
   Success: returns <0, 0, >0.
   Fail: invalid pointers => undefined behavior. */

int strncmp(const char *s1, const char *s2, size_t n);
/* Does: compare up to n chars.
   Success: returns <0, 0, >0.
   Fail: invalid pointers => undefined behavior. */

char *strcat(char *dest, const char *src);
/* Does: append src to end of dest (dest must be NUL-terminated and large enough).
   Success: returns dest.
   Fail: if dest too small => undefined behavior. */

char *strncat(char *dest, const char *src, size_t n);
/* Does: append up to n chars from src (writes terminating '\0').
   Success: returns dest.
   Fail: if dest too small => undefined behavior. */

char *strchr(const char *s, int c);
/* Does: find first occurrence of character c in string (including '\0').
   Success: returns pointer to match.
   Fail: returns NULL if not found. */

char *strstr(const char *haystack, const char *needle);
/* Does: find first occurrence of substring needle.
   Success: returns pointer to start of match (or haystack if needle empty).
   Fail: returns NULL if not found. */

char *strtok(char *str, const char *delim);
/* Does: split string into tokens (modifies input; uses internal static state).
   Success: returns pointer to next token.
   Fail: returns NULL when no more tokens. (Not thread-safe.) */

char *strerror(int errnum);
/* Does: map error number to human-readable message.
   Success: returns pointer to string (often static storage).
   Fail: no standard failure return; unknown errnum yields implementation-defined message. */

/* <stdio.h> */
int printf(const char *fmt, ...);
/* Does: formatted output to stdout.
   Success: returns number of characters printed.
   Fail: returns negative value. */

int fprintf(FILE *stream, const char *fmt, ...);
/* Does: formatted output to stream.
   Success: returns number of characters printed.
   Fail: returns negative value. */

int sprintf(char *str, const char *fmt, ...);
/* Does: formatted output to string (NO bounds checking).
   Success: returns number of characters written (excluding '\0').
   Fail: returns negative on encoding error; overflow is undefined behavior. */

int snprintf(char *str, size_t size, const char *fmt, ...);
/* Does: formatted output to string with buffer size limit.
   Success: returns number of chars that *would* have been written (excluding '\0'); output is truncated if return >= size.
   Fail: returns negative on encoding error. */

int scanf(const char *fmt, ...);
/* Does: formatted input from stdin.
   Success: returns number of input items assigned.
   Fail: returns EOF if input failure occurs before any assignment; otherwise returns count < expected. */

FILE *fopen(const char *path, const char *mode);
/* Does: open file stream.
   Success: returns FILE*.
   Fail: returns NULL and sets errno. */

int fclose(FILE *stream);
/* Does: close stream (flushes buffers).
   Success: returns 0.
   Fail: returns EOF (and sets errno). */

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
/* Does: read nmemb objects of `size`.
   Success: returns number of objects read (may be < nmemb at EOF).
   Fail: returns short count; use ferror(stream) to detect error vs feof(stream). */

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
/* Does: write nmemb objects.
   Success: returns number of objects written (should be nmemb).
   Fail: returns short count; use ferror(stream). */

char *fgets(char *s, int size, FILE *stream);
/* Does: read line up to size-1 chars, stores '\0'.
   Success: returns s.
   Fail/EOF: returns NULL (use feof/ferror to distinguish). */

int fputs(const char *s, FILE *stream);
/* Does: write string to stream (no automatic newline).
   Success: returns non-negative value.
   Fail: returns EOF. */

int fflush(FILE *stream);
/* Does: flush stream output buffer (or all streams if stream==NULL).
   Success: returns 0.
   Fail: returns EOF. */

void perror(const char *s);
/* Does: print s + ": " + strerror(errno) to stderr.
   Success: no return. */

/* <ctype.h> (note: pass unsigned char values or EOF to avoid UB) */
int isalpha(int c);  /* Does: alpha test. Success: nonzero if true. Fail: 0 if false. */
int isdigit(int c);  /* Does: digit test. Success: nonzero if true. Fail: 0 if false. */
int isalnum(int c);  /* Does: alnum test. Success: nonzero if true. Fail: 0 if false. */
int isspace(int c);  /* Does: whitespace test. Success: nonzero if true. Fail: 0 if false. */
int isupper(int c);  /* Does: upper test. Success: nonzero if true. Fail: 0 if false. */
int islower(int c);  /* Does: lower test. Success: nonzero if true. Fail: 0 if false. */
int tolower(int c);  /* Does: convert to lower. Success: converted or unchanged. */
int toupper(int c);  /* Does: convert to upper. Success: converted or unchanged. */

/* <math.h> (many set errno and/or floating exceptions on domain/range errors) */
double sqrt(double x);     /* Does: sqrt. Success: result. Fail: NaN for negative x. */
double pow(double x,double y); /* Does: power. Success: result. Fail: domain/range => NaN/Inf. */
double sin(double x);      /* Does: sine. Success: result. */
double cos(double x);      /* Does: cosine. Success: result. */
double tan(double x);      /* Does: tangent. Success: result. */
double exp(double x);      /* Does: e^x. Success: result. Fail: overflow => +Inf, errno may be set. */
double log(double x);      /* Does: ln(x). Success: result. Fail: x<=0 => NaN/-Inf, errno may be set. */
double floor(double x);    /* Does: round down. Success: result. */
double ceil(double x);     /* Does: round up. Success: result. */
double fabs(double x);     /* Does: absolute value. Success: result. */

/* <time.h> */
time_t time(time_t *tloc);
/* Does: get current calendar time.
   Success: returns current time; also stores in *tloc if non-NULL.
   Fail: returns (time_t)-1 (rare). */

clock_t clock(void);
/* Does: CPU time used by process.
   Success: returns clock ticks.
   Fail: returns (clock_t)-1. */

double difftime(time_t end, time_t begin);
/* Does: end-begin in seconds.
   Success: returns double seconds. */

struct tm *gmtime(const time_t *t);
/* Does: convert time_t to UTC broken-down time (static storage).
   Success: returns pointer.
   Fail: returns NULL. */

struct tm *localtime(const time_t *t);
/* Does: convert to local broken-down time (static storage).
   Success: returns pointer.
   Fail: returns NULL. */

size_t strftime(char *s,size_t max,const char *fmt,const struct tm *tm);
/* Does: format time into string.
   Success: returns bytes written (excluding '\0').
   Fail: returns 0 (buffer too small or error). */

/* <setjmp.h> */
int setjmp(jmp_buf env);
/* Does: save execution context.
   Success: returns 0 when saving.
   “Return” after longjmp: returns value passed to longjmp (or 1 if 0 passed). */

void longjmp(jmp_buf env,int val);
/* Does: restore context saved by setjmp.
   Success: does not return here; resumes at setjmp with return value val (or 1 if val==0). */

/* <signal.h> (C level) */
void (*signal(int signum, void (*handler)(int)))(int);
/* Does: set signal handler.
   Success: returns previous handler.
   Fail: returns SIG_ERR. */

int raise(int sig);
/* Does: send signal to current process.
   Success: returns 0.
   Fail: returns nonzero. */

/* ======================= POSIX ======================= */

/* <unistd.h> */
pid_t fork(void);
/* Does: create child process (duplicate).
   Success: returns 0 in child; returns child's PID in parent.
   Fail: returns -1 and sets errno. */

int execv(const char *path, char *const argv[]);
/* Does: replace current process with program at `path`.
   Success: does NOT return.
   Fail: returns -1 and sets errno. */

int execvp(const char *file, char *const argv[]);
/* Does: exec like execv but searches PATH for `file`.
   Success: does NOT return.
   Fail: returns -1 and sets errno. */

int execl(const char *path, const char *arg, ...);
/* Does: exec with variadic args (arg0, arg1, ..., NULL).
   Success: does NOT return.
   Fail: returns -1 and sets errno. */

void _exit(int status);
/* Does: exit process immediately (no stdio flush, no atexit).
   Success: does NOT return. */

pid_t getpid(void);
/* Does: get process ID.
   Success: returns PID (never fails in practice). */

pid_t getppid(void);
/* Does: get parent PID.
   Success: returns parent PID (never fails in practice). */

ssize_t read(int fd, void *buf, size_t count);
/* Does: read up to count bytes from fd.
   Success: returns number of bytes read (0 means EOF).
   Fail: returns -1 and sets errno. */

ssize_t write(int fd, const void *buf, size_t count);
/* Does: write up to count bytes to fd.
   Success: returns number of bytes written (may be < count).
   Fail: returns -1 and sets errno. */

int close(int fd);
/* Does: close file descriptor.
   Success: returns 0.
   Fail: returns -1 and sets errno. */

off_t lseek(int fd, off_t offset, int whence);
/* Does: change file offset (SEEK_SET/CUR/END).
   Success: returns new offset.
   Fail: returns (off_t)-1 and sets errno. */

unsigned sleep(unsigned seconds);
/* Does: sleep for seconds.
   Success: returns 0 if slept full duration.
   Fail/Interrupted: returns remaining seconds. */

int pipe(int pipefd[2]);
/* Does: create pipe (pipefd[0]=read end, pipefd[1]=write end).
   Success: returns 0.
   Fail: returns -1 and sets errno. */

/* <sys/wait.h> */
pid_t wait(int *status);
/* Does: wait for any child to change state (usually exit).
   Success: returns child PID; *status filled.
   Fail: returns -1 and sets errno. */

pid_t waitpid(pid_t pid, int *status, int options);
/* Does: wait for specific child (pid) or rules (pid=-1,0,< -1).
   Success: returns child PID (or 0 if WNOHANG and none).
   Fail: returns -1 and sets errno. */

int WIFEXITED(int status);
/* Does: test if child exited normally.
   Success: nonzero if true, 0 if false. */

int WEXITSTATUS(int status);
/* Does: extract child exit code.
   Success: 0..255 value (ONLY valid if WIFEXITED true). */

int WIFSIGNALED(int status);
/* Does: test if child terminated by signal.
   Success: nonzero if true, 0 if false. */

int WTERMSIG(int status);
/* Does: extract terminating signal number.
   Success: signal number (ONLY valid if WIFSIGNALED true). */

/* <fcntl.h> */
int open(const char *path, int flags, ...);
/* Does: open file and return fd.
   Success: returns fd (>=0).
   Fail: returns -1 and sets errno. */

int openat(int dirfd, const char *path, int flags, ...);
/* Does: open relative to dirfd.
   Success: returns fd.
   Fail: returns -1 and sets errno. */

int fcntl(int fd, int cmd, ...);
/* Does: manipulate fd flags/locks (depends on cmd).
   Success: returns cmd-specific value (often 0 or >=0).
   Fail: returns -1 and sets errno. */

/* <sys/stat.h> */
int stat(const char *path, struct stat *buf);
/* Does: get file metadata by path.
   Success: returns 0; buf filled.
   Fail: returns -1 and sets errno. */

int fstat(int fd, struct stat *buf);
/* Does: get metadata by fd.
   Success: 0; Fail: -1 + errno. */

int lstat(const char *path, struct stat *buf);
/* Does: like stat but for symlink itself.
   Success: 0; Fail: -1 + errno. */

int mkdir(const char *path, mode_t mode);
/* Does: create directory.
   Success: 0.
   Fail: -1 + errno. */

int chmod(const char *path, mode_t mode);
/* Does: change permissions.
   Success: 0.
   Fail: -1 + errno. */

mode_t umask(mode_t mask);
/* Does: set process file-creation mask.
   Success: returns previous mask. */

/* <dirent.h> */
DIR *opendir(const char *name);
/* Does: open directory stream.
   Success: returns DIR*.
   Fail: returns NULL and sets errno. */

struct dirent *readdir(DIR *dirp);
/* Does: read next directory entry.
   Success: returns pointer to entry.
   Fail/End: returns NULL (errno may indicate error; check errno). */

int closedir(DIR *dirp);
/* Does: close directory stream.
   Success: returns 0.
   Fail: returns -1 and sets errno. */

void rewinddir(DIR *dirp);
/* Does: reset directory stream to beginning.
   Success: no return. */

/* <signal.h> (POSIX) */
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
/* Does: install/get signal action.
   Success: returns 0.
   Fail: returns -1 and sets errno. */

int sigemptyset(sigset_t *set);
/* Does: initialize empty signal set.
   Success: 0.
   Fail: -1 + errno. */

int sigaddset(sigset_t *set, int signum);
/* Does: add a signal to set.
   Success: 0.
   Fail: -1 + errno. */

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
/* Does: change blocked-signal mask.
   Success: 0.
   Fail: -1 + errno. */

int kill(pid_t pid, int sig);
/* Does: send signal to process (or process group).
   Success: 0.
   Fail: -1 + errno. */

/* <pthread.h> (POSIX threads) */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);
/* Does: start a new thread running start(arg).
   Success: returns 0; *thread set.
   Fail: returns error number (NOT -1); errno not required. */

int pthread_join(pthread_t thread, void **retval);
/* Does: wait for thread to finish.
   Success: returns 0; *retval receives thread’s return value if non-NULL.
   Fail: returns error number. */

void pthread_exit(void *retval);
/* Does: terminate calling thread, making retval available to join.
   Success: does not return. */

pthread_t pthread_self(void);
/* Does: get calling thread ID.
   Success: returns pthread_t (no failure). */

int pthread_cancel(pthread_t thread);
/* Does: request cancellation of a thread.
   Success: returns 0.
   Fail: returns error number. */

int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a);
/* Does: initialize mutex.
   Success: 0.
   Fail: error number. */

int pthread_mutex_lock(pthread_mutex_t *m);
/* Does: lock mutex (may block).
   Success: 0.
   Fail: error number. */

int pthread_mutex_unlock(pthread_mutex_t *m);
/* Does: unlock mutex.
   Success: 0.
   Fail: error number. */

/* <semaphore.h> */
int sem_init(sem_t *sem, int pshared, unsigned value);
/* Does: initialize unnamed semaphore.
   Success: 0.
   Fail: -1 + errno. */

int sem_wait(sem_t *sem);
/* Does: decrement semaphore (block if zero).
   Success: 0.
   Fail: -1 + errno. */

int sem_post(sem_t *sem);
/* Does: increment semaphore.
   Success: 0.
   Fail: -1 + errno. */

int sem_destroy(sem_t *sem);
/* Does: destroy unnamed semaphore.
   Success: 0.
   Fail: -1 + errno. */

/* <sys/mman.h> */
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
/* Does: map files/anon memory into address space.
   Success: returns mapped address.
   Fail: returns MAP_FAILED and sets errno. */

int munmap(void *addr, size_t len);
/* Does: unmap mapping.
   Success: 0.
   Fail: -1 + errno. */

int mprotect(void *addr, size_t len, int prot);
/* Does: change page protections.
   Success: 0.
   Fail: -1 + errno. */

int msync(void *addr, size_t len, int flags);
/* Does: flush mapping to backing store.
   Success: 0.
   Fail: -1 + errno. */

/* <sys/socket.h> */
int socket(int domain, int type, int protocol);
/* Does: create socket.
   Success: returns fd.
   Fail: -1 + errno. */

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
/* Does: bind local address.
   Success: 0.
   Fail: -1 + errno. */

int listen(int sockfd, int backlog);
/* Does: mark socket as passive (server).
   Success: 0.
   Fail: -1 + errno. */

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
/* Does: accept incoming connection.
   Success: returns new connected fd.
   Fail: -1 + errno. */

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
/* Does: connect to remote.
   Success: 0.
   Fail: -1 + errno. */

ssize_t send(int sockfd, const void *buf, size_t len, int flags);
/* Does: send bytes on socket.
   Success: returns bytes sent (may be < len).
   Fail: -1 + errno. */

ssize_t recv(int sockfd, void *buf, size_t len, int flags);
/* Does: receive bytes.
   Success: returns bytes received (0 means peer closed).
   Fail: -1 + errno. */

/* <arpa/inet.h> */
uint16_t htons(uint16_t x);
/* Does: host→network byte order (16-bit).
   Success: returns converted value. */

uint32_t htonl(uint32_t x);
/* Does: host→network (32-bit). Success: converted value. */

uint16_t ntohs(uint16_t x);
/* Does: network→host (16-bit). Success: converted value. */

uint32_t ntohl(uint32_t x);
/* Does: network→host (32-bit). Success: converted value. */

int inet_pton(int af, const char *src, void *dst);
/* Does: parse text IP to binary.
   Success: returns 1.
   Fail: returns 0 (invalid text) or -1 (af unsupported, errno set). */

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
/* Does: binary IP to text.
   Success: returns dst.
   Fail: returns NULL and sets errno. */

/* <poll.h> / <sys/select.h> */
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
/* Does: wait for events on fds.
   Success: returns number of fds with events (0 on timeout).
   Fail: -1 + errno. */

int select(int nfds, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv);
/* Does: wait for readiness on fd sets.
   Success: returns number ready (0 on timeout).
   Fail: -1 + errno. */

/* <sys/time.h> */
int gettimeofday(struct timeval *tv, struct timezone *tz);
/* Does: get wall-clock time (tz usually ignored).
   Success: 0.
   Fail: -1 + errno. */

int settimeofday(const struct timeval *tv, const struct timezone *tz);
/* Does: set system time (usually requires privileges).
   Success: 0.
   Fail: -1 + errno. */

/* <sys/resource.h> */
int getrlimit(int resource, struct rlimit *rlim);
/* Does: get resource limits.
   Success: 0.
   Fail: -1 + errno. */

int setrlimit(int resource, const struct rlimit *rlim);
/* Does: set resource limits (may require privileges).
   Success: 0.
   Fail: -1 + errno. */

/* ======================= END ======================= */
