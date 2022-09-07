#include <stdio.h>
#include <pthread.h>
int main() {
    printf("Size of pthread_t %ld.\n", sizeof(pthread_t));
    return 0;
}
