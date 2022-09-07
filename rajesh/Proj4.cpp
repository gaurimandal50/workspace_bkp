#include <string>
#include <iostream>
#include <iostream>
using namespace std;
#define MAX_FILE_NAME 20
#define PATH "/home/tarzan/test/"
int strlen(char *s) {
   int i = 0;
   for(;s[i];i++);
   return i;
}

int strncpy(char *d, char *s, int len) {
   int i = 0; 
   for(; i < len; i++) {
       *(d+i) = *(s+i);
   }
   return 0;
}

char** getfiles() {
   char command[100] = {0};
   FILE *fp = NULL;
   char file[24] = {0};
   char filelist[20][24] = {0};
   string cur_string = "";
   sprintf(command, "ls %s", PATH);
   fp = popen(command, "r");
   if(!fp) {
       cout<<"Failed to get filenames"<<endl;
       return NULL;
   }

   int i = 0;
   while(fgets(file, MAX_FILE_NAME, fp)) {
       strncpy(filelist[i], file, strlen(file));
       i++;
   }

   for(int j = 0; j < i; j++){
     cout<<filelist[j]<<endl;
   }
   pclose(fp);
   return filelist;
}

int main() {
   char **filelist;
   filelist = getfiles();
   find_cost_matrix();
   return 0;
}
