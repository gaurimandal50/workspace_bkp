#include <stdio.h>
struct test {
    char a;
    char b;
    char c;
}__randomize_layout;

int main(){
    struct test t = { 'a', 'b', 'c'};
    char *c = (char*)&t;
    //char t[] = { 'a','b','c'};
   // char *c = t;
    printf("Sequence = %c %c , %c %c %c.\n",c[1], c[2], *c, *(c++), *(c++));
    //printf("%c %c %c\n", t[0], t[1], t[2]);
    return 0;
}
