#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    //char buff[64] = {0};
    char *buff = NULL;
    int count = 1;
    int i=0;
    int fd = open("/proc/tarzanproc", O_RDONLY);
    if(fd<0) {
	printf("Failed to open file. %m\n");
	return -1;
    }

    for(i = 0;i < 10; i++) {    
    count = read(fd, buff, 10);
    if(count < 0) {
	printf("Read failed. %m\n");
	return -1;
    }
    	printf("Received %s. count %d.\n", buff, count);
    	memset(buff, 0, sizeof(buff));
    }
    close(fd);
    return 0;
}
