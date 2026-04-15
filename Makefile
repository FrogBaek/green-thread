# 그린 스레드 프로젝트 Makefile (Windows / MinGW-w64)
# 빌드: make
# 실행: make run
# 정리: make clean

CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = green_thread.exe

SRCS = main.c green_thread.c
ASM = context_switch.S

all: $(TARGET)

$(TARGET): $(SRCS) $(ASM)
	$(CC) $(CFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
