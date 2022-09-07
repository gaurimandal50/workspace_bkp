readelf Tscull.ko

gdb -g example.c
Makefile needs these lines to compile module in debugger mode:-
MY_CFLAGS += -g -DDEBUG
ccflags-y += ${MY_CFLAGS}
CC += ${MY_CFLAGS}
