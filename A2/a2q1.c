// Haojian Wang 300411829 Xinran Xie 300452622
// Group 77 by CSI 3131

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int is_prime(int n){
    if (n < 2) return 0;

    for (int i = 2; i * i <= n; i++) {
        if( n % i == 0) return 0;
    }
    return 1;
}

void *print_primes(void *arg) {
    int limit = *((int *)arg);
    printf("Prime numbers up to %d:\n", limit);
    for (int i = 2; i <= limit; i++) {
        if (is_prime(i)) {
            printf("%d \n", i);
        }
    }
    printf("\n");
    return NULL;
}

/*
 * Main 主函数，首先判断Args
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int limit = atoi(argv[1]);
    pthread_t tid;

    pthread_create(&tid, NULL, print_primes, &limit);
    pthread_join(tid, NULL);

    return 0;
}
