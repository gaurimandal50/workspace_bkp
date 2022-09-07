#include <stdio.h>
#include <stdint.h>
//#define intptr_t intptr_t
struct octt { /*size of octt must be same as that of void* */
    //char a[8];
    unsigned long a;
}__attribute__((packed));

int main(){
    intptr_t v = 100;
    printf("Value of v is %d\n", (int)v);
    int a = 500;
    int *p = &a;
    printf("Address of a is value of p is %p = %p.\n", &a, p);
    //v = (intptr_t)p;
    v = p; /*Works with intptr_t and unsigned long type. Always use intptr_t if needed */
    //struct octt wow = (struct octt)p; Doesn't work
    int *q = (int*) v;
    printf("Original value of a %d address of a %p.\n", *q, q);
    printf("sizeof intptr_t %ld void* %ld intptr_t %ld.\n", sizeof(intptr_t), sizeof(void*), sizeof(intptr_t));
    return 0;
}
