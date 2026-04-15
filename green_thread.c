#include "green_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GreenThread threads[MAX_THREADS];
static int thread_count = 0;
static int current_thread = -1;
static void *scheduler_sp = NULL;

static void thread_wrapper(void) {
    threads[current_thread].entry();
    threads[current_thread].state = FINISHED;
    context_switch(&threads[current_thread].sp, scheduler_sp);
}

void scheduler_init(void) {
    thread_count = 0;
    current_thread = -1;
    scheduler_sp = NULL;
    memset(threads, 0, sizeof(threads));
}

int green_thread_create(void (*func)(void)) {
    if (thread_count >= MAX_THREADS) {
        fprintf(stderr, "error: 최대 스레드 수(%d) 초과\n", MAX_THREADS);
        return -1;
    }

    GreenThread *t = &threads[thread_count];
    t->id = thread_count;
    t->state = READY;
    t->entry = func;

    t->stack = malloc(STACK_SIZE);
    if (!t->stack) {
        fprintf(stderr, "error: 스택 메모리 할당 실패\n");
        return -1;
    }

    void **sp = (void **)((char *)t->stack + STACK_SIZE);

    // Windows 에서 unsigned long 은 32비트라 ~0xFUL 로 마스킹하면 상위 32비트가
    // 날아간다. 반드시 uintptr_t 폭으로 마스킹해야 포인터가 망가지지 않는다.
    sp = (void **)((uintptr_t)sp & ~(uintptr_t)0xF);

    *(--sp) = (void *)thread_wrapper;
    // context_switch.S 의 pop 순서 역순. Windows x64 는 rdi, rsi 도
    // callee-saved 라서 Linux(6개) 와 달리 8개를 저장한다.
    *(--sp) = 0;  // rbx
    *(--sp) = 0;  // rbp
    *(--sp) = 0;  // rdi
    *(--sp) = 0;  // rsi
    *(--sp) = 0;  // r12
    *(--sp) = 0;  // r13
    *(--sp) = 0;  // r14
    *(--sp) = 0;  // r15

    t->sp = sp;

    printf("[scheduler] 스레드 #%d 생성 (스택: %p, 크기: %dKB)\n",
           thread_count, t->stack, STACK_SIZE / 1024);

    return thread_count++;
}

void green_thread_yield(void) {
    context_switch(&threads[current_thread].sp, scheduler_sp);
}

void scheduler_run(void) {
    while (1) {
        int found = 0;

        for (int i = 0; i < thread_count; i++) {
            if (threads[i].state != READY)
                continue;

            found = 1;
            current_thread = i;
            threads[i].state = RUNNING;

            context_switch(&scheduler_sp, threads[i].sp);

            if (threads[i].state == RUNNING) {
                threads[i].state = READY;
            }
        }

        if (!found) break;
    }

    for (int i = 0; i < thread_count; i++) {
        free(threads[i].stack);
        threads[i].stack = NULL;
    }
}
