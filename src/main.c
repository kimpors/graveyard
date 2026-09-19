#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUFLEN 256

int main(void) 
{
  printf("### Graveyard ###\n");

  char buf[BUFLEN];

  do 
  {
    printf("> ");
    scanf("%255s", buf);

    if (!strcmp(buf, ";exit"))
    {
      break;
    }
    else 
    {
      printf("Unexpected command: '%s'\n", buf);
    }
  }while (true);

  return 0;
}
