#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){

    int ret;
    char a_array[] = "0123456789ABCDEF";
    int i, j, k, l, m, n, o, p;
    char cmd[] = "./aes -d -1 ";
    char key[4];
//    char key_check[4] = "ABCD";
    char options[] = " < test.zip.crypt";
    char* full_cmd;
    size_t cmd_len = strlen(cmd);
    size_t options_len = strlen(options);
    size_t full_cmd_len = cmd_len + options_len;
    FILE *fp;
    char path[1024];
    int status;
    char first_byte, second_byte, third_byte, fourth_byte;
 
    full_cmd = (char *) malloc(full_cmd_len*sizeof(char));

    for(i = 0; i < 11; i++){
        for(j = 0; j < 12; j++){
            for(k = 0; k < 13; k++){
                for(l = 0; l < 14; l++){
    //                bzero(key,sizeof(key));
                    key[0] = a_array[i];
                    key[1] = a_array[j];
                    key[2] = a_array[k];
                    key[3] = a_array[l];
 
                //    printf("%c %c %c %c \n", key[0], key[1], key[2], key[3]);
                }
            }
        }
    }
//   printf("key_check = %s\n", key_check);
   printf("key = %s\n", key);

   full_cmd = (char *) strcat(full_cmd, cmd);
//   full_cmd = (char *) strcat(full_cmd, key_check);
   full_cmd = (char *) strcat(full_cmd, key);
   full_cmd = (char *) strcat(full_cmd, options);

   // For loop that runs through all possible permutations of
   // the 8 byte sequence in total 12870 iterations
   printf("Full command = %s\n", full_cmd);

   /* Open the command for reading. */
   fp = popen(full_cmd, "r");
   if (fp == NULL) {
      printf("Failed to run command\n" );
      exit(0);
   }
   //next up: Write a for loop that runs through each of the possible keys
   // create a new file with a key value of something like fedc or something...
   /* Read the output a line at a time - output it. 8 Hex chars I think. */
   fgets(path, sizeof(path)-1, fp);
      
   if(i == 0){
       first_byte = path[0];
       second_byte = path[1];
       third_byte = path[2];
       fourth_byte = path[3];
       if( (first_byte == 0x50) && (second_byte == 0x4b) && (third_byte == 0x03) && (fourth_byte == 0x04))printf("truf\n");
   }

   /* close */
   pclose(fp);

   printf("\n");


   free(full_cmd);
   return 0;

}
