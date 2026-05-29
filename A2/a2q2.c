//
// Created by Jace Wang on 2026-05-28.
// Group 77
// Haojian Wang 300411829, Xinran Xie 300452622

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int fib_array[MAX];
int n;

void *generate_fibonacci(void *arg) {
    if (n >= 1) fib_array[0] = 0;
    if (n >= 2) fib_array[1] = 1;

    for (int i = 2; i < n; i++) {
        fib_array[i] = fib_array[i - 1] + fib_array[i - 2];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    if (n <= 0 || n > MAX) {
        printf("Enter a number between 1 and %d.\n", MAX);
        return 1;
    }

    pthread_t tid;

    pthread_create(&tid, NULL, generate_fibonacci, NULL);
    pthread_join(tid, NULL);

    printf("Fibonacci sequence:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", fib_array[i]);
    }
    printf("\n");
    return 0;
}
