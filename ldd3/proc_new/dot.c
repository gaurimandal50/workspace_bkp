#include <stdio.h>
#define printf(fmt, ...) printf(fmt, __VA_ARGS__)
//#define print(fmt, varg...) printf(fmt, varg)
int fun(int a, ...) {
   printf("value of a %d.\n", a);
   return 0;
}
int main() {
  fun(5, 'a');
  return 0;
}
