#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/socket.h>

#include "Tscull.h"

int major = 237; /*It may differ for different loadings of same modules*/
int main() {
   int ret = 0;
   char buf[16] = {0};
   int scull = 300;
   int sculldup = 0;
   printf("Hoping you made sure that major number of the scull0 is %d.\n", major);
   int fd = open("/dev/scull0", O_RDWR);
   if(fd < 0) {
     printf("open failed. returned %d %m\n", fd);
     return -1;
   }
   sleep(20);
   if(ioctl(fd, SCULL_SET_PARAM, &scull) < 0) {
       printf("ioctl failed, returned %m.\n");
       ret = close(fd);
       return -1;
   }
   printf("Setting of ioctl successful.\n");
   sleep(20);
   if(ioctl(fd, SCULL_GET_PARAM, &sculldup) < 0) {
       printf("ioctl failed, returned %m.\n");
       ret = close(fd);
       return -1;
   }
   printf("Scull ioctl returned %d.\n", sculldup);
   ret = close(fd);
   printf("close returned %d %m.\n", ret);
   return 0;
}
