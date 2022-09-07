#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int c = 0;
    int fd = open("/dev/input/mouse1", O_RDONLY);
    if(fd < 0){
        printf("Error opending /dev/ttyACM0. Please run with sudo.\n");
        return -1;
    }

    while(read(fd, &c, 1) != EOF) {
        printf("%d", c);
        c = 0;
        usleep(100);
    }

    close(fd);
    return 0;
}
