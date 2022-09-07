#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

struct test {
  int a;
  char b;
  char b1;
  int c;
  char d[5];
}__attribute__((packed));

int main(){
    struct test t = { .b1 = 'k' };
    printf("Offset of a %ld.\n", offsetof(struct test, a));
    printf("Offset of b %ld.\n", offsetof(struct test, b));
    printf("Offset of b1 %ld.\n", offsetof(struct test, b1));
    printf("Offset of c %ld.\n", offsetof(struct test, c));
    printf("Offset of d %ld.\n", offsetof(struct test, d));
    printf("Address of t %p c %p.\n", &t, &t.c);
    printf("Address of t from offset %p.\n", (uint8_t*)&t.c - offsetof(struct test, c));
    printf("Dereferencing b1 =%c \n", ((struct test *)((uint8_t*)&t.c - offsetof(struct test, c)))->b1);
    return 0;
}
