#include <stdio.h>
#include <pthread.h>
void *counter(void *arg) {
    int loop = 1;
    int j = 0;
    for( ; loop; j++) {
        printf("THREAD 1 counter %d.\n", j);
    }

    return NULL;
}

void *multiplier(void *arg) {
    int loop = 1;
    int j = 0;
    for( ; loop; j++) {
        printf("THREAD 2 multiple %d.\n", j*100);
    }

    return NULL;
}

int negative_counter() {
    int loop = 1;
    int j = 0;
    for( ; loop; j--) {
        printf("THREAD MAIN counter %d.\n", j);
    }

    return 0;
}

int main() {
    pthread_t t1 = {0};
    pthread_t t2 = {0};
    printf("Main thread started.\n");
    pthread_create(&t1, NULL, counter, NULL);
    pthread_create(&t2, NULL, multiplier, NULL);

    negative_counter();

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Both threads exited. Exiting main thread.\n");
    return 0;
}
