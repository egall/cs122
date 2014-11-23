#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* record type to allocate on heap */
typedef struct chunk {
    char inp[64];            /* vulnerable input buffer */
    void (*process)(char *); /* pointer to function to process inp */
} chunk_t;
void showlen(char *buf)
{
    int len;
    len = strlen(buf);
    printf("buffer5 read %d chars\n", len);
}
int main(int argc, char *argv[])
{
    chunk_t *next;
    setbuf(stdin, NULL);
    next = malloc(sizeof(chunk_t));
    next->process = showlen;
    printf("Enter value: ");
    fgets(next->inp, 63, stdin);
    next->process(next->inp);
    printf("buffer5 done\n");
}
