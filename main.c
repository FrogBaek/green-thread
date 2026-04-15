#include "green_thread.h"
#include <stdio.h>

void task_a(void) {
    for (int i = 0; i < 5; i++) {
        printf("[스레드 A] 작업 중... (%d/5)\n", i + 1);
        green_thread_yield();
    }
    printf("[스레드 A] 완료!\n");
}

void task_b(void) {
    for (int i = 0; i < 3; i++) {
        printf("[스레드 B] 작업 중... (%d/3)\n", i + 1);
        green_thread_yield();
    }
    printf("[스레드 B] 완료!\n");
}

void task_c(void) {
    for (int i = 0; i < 4; i++) {
        printf("[스레드 C] 작업 중... (%d/4)\n", i + 1);
        green_thread_yield();
    }
    printf("[스레드 C] 완료!\n");
}

int main(void) {
    printf("=== 그린 스레드 (Green Thread) 데모 ===\n");
    printf("OS 스레드 1개 위에서 그린 스레드 3개를 실행합니다.\n\n");

    scheduler_init();

    green_thread_create(task_a);
    green_thread_create(task_b);
    green_thread_create(task_c);

    printf("\n스케줄러 시작 (라운드 로빈)\n");
    printf("────────────────────────────\n");

    scheduler_run();

    printf("────────────────────────────\n");
    printf("모든 스레드 실행 완료.\n");

    return 0;
}
