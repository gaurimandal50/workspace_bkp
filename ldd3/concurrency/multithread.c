#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

struct align {
    char c;
    int k;
    char d;
    uint64_t i;
};//__attribute__((packed));
struct align a = {0};

void *counter(void *arg) {
    int loop = 1;
    int j = 0;
    for(;;) {
        a.i = 100;
    }

    return NULL;
}

void *multiplier(void *arg) {
    int loop = 1;
    int j = 0;
    for( ;;) {
        if(a.i != 100) {
            printf("Value of a %d.\n", a.i);
        }
    }

    return NULL;
}

int main() {
    pthread_t t1 = {0};
    pthread_t t2 = {0};
    printf("Main thread started. Address %p %p\n", &a.c, &a.i);
    pthread_create(&t1, NULL, counter, NULL);
    pthread_create(&t2, NULL, multiplier, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Both threads exited. Exiting main thread.\n");
    return 0;
}
