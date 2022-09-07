#include <stdio.h>
#include <stdint.h>
struct gen {
    unsigned long v;
    //intptr_t v;
};

int main() {
    struct gen p = {0};
    int a = 100;
    printf("Original address of a %p and value of a %d.\n", &a, a);
    p.v = (unsigned long)&a;
    printf("Value %d address %p\n", *(int*)p.v, (void *)p.v);
    p.v = 500;
    printf("Value %ld.\n", p.v);
    printf("WOW unsigned long successfully stored value as well as address.\n");
    //WARNING:Whenever needed to do so, use intptr_t instead of unsigned long*/
    return 0;
}
