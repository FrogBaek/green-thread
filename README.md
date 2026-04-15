# green-thread-impl

C 언어로 직접 구현한 Green Thread (사용자 수준 스레드).

OS 스레드 1개 위에서 여러 개의 사용자 수준 스레드를 만들어 협력형(cooperative)으로 번갈아 실행합니다. Java Virtual Thread 가 내부적으로 어떤 메커니즘으로 동작하는지를 이해하기 위해, 핵심인 컨텍스트 스위칭·스택 관리·스케줄링을 바이트 단위로 직접 작성한 학습용 프로젝트입니다.

## 구성

| 파일 | 역할 |
|---|---|
| `green_thread.h` | TCB 구조체, 상태 enum, 공개 API 선언 |
| `green_thread.c` | 스케줄러, 스레드 생성, `yield` (순수 C) |
| `context_switch.S` | callee-saved 레지스터 저장/복구 + 스택 포인터 교체 (어셈블리) |
| `main.c` | 3개의 그린 스레드를 라운드 로빈으로 실행하는 데모 |
| `Makefile` | 빌드 / 실행 / 정리 |

전체 코드의 대부분은 C 이고, 현재 CPU 의 레지스터를 스택에 밀어 넣고 스택 포인터 자체를 바꾸는 `context_switch` 한 함수만 C 의 추상화 바깥에 있어 어셈블리로 작성했습니다.

## 빌드 및 실행

Windows / MinGW-w64 기준입니다.

```bash
make        # 빌드
make run    # 빌드 후 실행
make clean  # 산출물 정리
```

## 관련 글

이 프로젝트의 설계 의도와 구현 디테일(왜 callee-saved 만 저장하면 되는지, 스택 포인터 한 줄 교체가 어떻게 "스레드 전환"이 되는지 등) 은 블로그 글에 정리해 두었습니다.

- Virtual Thread 는 어떻게 동작하는가 — C 로 직접 구현해보기
