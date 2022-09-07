#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
   int ret = 0;
   char buf[16] = {0};
   int fd = open("/dev/scull0", O_RDWR);
   printf("open returned %d %m\n", fd);
   ret = write(fd, "TARZANTARZaeiou", 10);
   printf("write ret %d %m.\n", ret);
   printf("Holding kernel lock for 60 second to test the completion working. And how close behaves\n");
   sleep(60);
   ret = close(fd);
   printf("close returned %d %m.\n", ret);
   return 0;
}
