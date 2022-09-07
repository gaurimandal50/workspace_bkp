#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define SIZE 20
int main() {
    int arr[SIZE] = { 65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84};
    int *p = arr;
    int i = 0;
    int result = 0;
    for(; i<3; i++) {
        for(int j = 0; j< SIZE; j++) {
            result += arr[j];
            sleep(1);
            if(j == 7)
                *(int*)0 = 5; //dereferencing null pointer to generate SIGSEGV
        }
    }
    printf("Result %d.\n",result);
    return 0;
}
