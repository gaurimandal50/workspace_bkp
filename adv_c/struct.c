#include <stdio.h>
struct test {
    char a;
    char b;
    char c;
};

/*Mehod 1 of statically initializing structure*/
struct test t = {
    .a = 'm',
    .c = 'x',
    .b = 'z'
};

/*Mehod 2 of statically initializing structure*/
struct test t1 = {
    a : 'm',
    c : 'x',
    b : 'z'
};

/*Mehod 3 of statically initializing structure*/
struct test t2 = {
    'm',
    'z',
    'x'
};

int main(){
    printf("values %c %c %c\n", t.a, t.b, t.c);
    printf("values %c %c %c\n", t1.a, t1.b, t1.c);
    printf("values %c %c %c\n", t2.a, t2.b, t2.c);
    return 0;
}
