#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){

   int ret;
   char cmd[] = "./aes -d -1 ";
   char key[4] = "abcd";
   char options[] = " < test.zip.crypt";
//   char outfile[] = ;
   char* full_cmd;
   size_t cmd_len = strlen(cmd);
   size_t options_len = strlen(options);
   size_t full_cmd_len = cmd_len + options_len;
   FILE *fp;
   char path[1024];
   int status;
   char first_byte, second_byte, third_byte, fourth_byte;
   int i = 0;

   full_cmd = (char *) malloc(full_cmd_len*sizeof(char));

   printf("size of char = %lu\n", sizeof(char));

   full_cmd = (char *) strcat(full_cmd, cmd);
   full_cmd = (char *) strcat(full_cmd, key);
   full_cmd = (char *) strcat(full_cmd, options);

   // For loop that runs through all possible permutations of
   // the 8 byte sequence in total 12870 iterations

   /* Open the command for reading. */
   fp = popen(full_cmd, "r");
   if (fp == NULL) {
      printf("Failed to run command\n" );
      exit(0);
   }
   //next up: Write a for loop that runs through each of the possible keys
   // create a new file with a key value of something like fedc or something...
   /* Read the output a line at a time - output it. 8 Hex chars I think. */
   while (fgets(path, sizeof(path)-1, fp) != NULL) {
      
      if(i == 0){
          first_byte = path[0];
          second_byte = path[1];
          third_byte = path[2];
          fourth_byte = path[3];
          if( (first_byte == 0x50) && (second_byte == 0x4b) && (third_byte == 0x03) && (fourth_byte == 0x04))printf("truf\n");
      }
      if(first_byte == 0x04)printf("found first byte\n");
//      printf("%s", path);
      i++;
   }

   /* close */
   pclose(fp);

   printf("\n");


   free(full_cmd);
   return 0;

}
