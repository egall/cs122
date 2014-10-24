#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){

   int ret;
   char cmd[] = "./aes -d -1 ";
   char options[] = "abcd";
   char* full_cmd;
   size_t cmd_len = strlen(cmd);
   size_t options_len = strlen(options);
   size_t full_cmd_len = cmd_len + options_len;

   full_cmd = (char *) malloc(full_cmd_len*sizeof(char));


   full_cmd = (char *) strcat(full_cmd, cmd);
   full_cmd = (char *) strcat(full_cmd, options);

   // For loop that runs through all possible permutations of
   // the 8 byte sequence in total 12870 iterations


   // Do I need to check output?

   ret = system(full_cmd);

   free(full_cmd);

}
