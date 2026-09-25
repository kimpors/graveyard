#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "table.h"
#include "command.h"
#include "statement.h"
#include "input-buffer.h"

// int readline(InputBuffer *buf)
// {
//   ssize_t readed;
//   readed = getline(&(buf->data), &(buf->cap), stdin);
//
//     if (readed <= 0)
//     {
//       puts("Error reading input");
//       free_input_buffer(buf);
//       return EXIT_FAILURE;
//     }
//
//     buf->len = readed - 1;
//     buf->data[readed - 1] = '\0';
//
//   return EXIT_SUCCESS;
// }


int main(void) 
{
  Table *tb = init_table();
  InputBuffer *buf = init_input_buffer();

  printf("### Graveyard ###\n");

  do 
  {
    printf("> ");
    if (readline(buf))
    {
      free_input_buffer(buf);
      free_table(tb);
      exit(EXIT_FAILURE);
    }
    // readed = getline(&(buf->data), &(buf->cap), stdin);
    //
    // if (readed <= 0)
    // {
    //   puts("Error reading input");
    //   free_input_buffer(buf);
    //   free_table(tb);
    //   exit(EXIT_FAILURE);
    // }
    //
    // buf->len = readed - 1;
    // buf->data[readed - 1] = '\0';

    if (buf->data[0] == ';')
    {
      switch (execute_command(buf))
      {
        case COMMAND_SUCCESS: break;
        case COMMAND_UNRECOGNIZED:
          printf("[Error]Unrecognized keyword int a command: '%s'\n", buf->data);
        continue;
      }
    }

    Statement st;
    switch(prepare_statement(buf, &st))
    {
      case PREPARE_SUCCESS: break;
      case PREPARE_SYNTAX_ERROR:
        printf("[Error]Could not parse statement: '%s'\n", buf->data);
      continue;
      case PREPARE_UNRECOGNIZED:
        printf("[Error]Unrecognized keyword in a statement: '%s'\n", buf->data);
      continue;
    }

    switch(execute_statement(&st, tb))
    {
      case EXECUTE_SUCCESS:
        puts("(Executed)");
      break;
      case EXECUTE_TABLE_FULL:
        printf("[Error]Table is full");
      break;
    }
  }while (true);

  free_input_buffer(buf);
  free_table(tb);
  return 0;
}
