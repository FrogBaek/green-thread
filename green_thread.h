#ifndef GREEN_THREAD_H
#define GREEN_THREAD_H

#include <stdint.h>

#define STACK_SIZE  (1024 * 64)
#define MAX_THREADS 128

typedef enum {
    READY,
    RUNNING,
    FINISHED
} ThreadState;

typedef struct {
    int         id;
    ThreadState state;
    void       *stack;
    void       *sp;
    void      (*entry)(void);
} GreenThread;

void scheduler_init(void);
int  green_thread_create(void (*func)(void));
void green_thread_yield(void);
void scheduler_run(void);

void context_switch(void **old_sp, void *new_sp);

#endif
