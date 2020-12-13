#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

int get_input(char *str) {
  char buf[120];

  scanf("%[^\n]%*c", buf);
  // printf("%s", buf);
  if (strlen(buf) != 0) {
    strcpy(str, buf);
    return 0;
  } else {
    return 1;
  }
}

int parse_input(char *string_input, char *string_command, char *string_echo, int *arg_id) {
  char delimiter[] = " ";
  char *remainder, *context, *temp;

  int length = strlen(string_input);
  char *input_copy = (char*) calloc(length + 1, sizeof(char));
  strncpy(input_copy, string_input, length);

  temp = strtok_r (input_copy, delimiter, &context);

  strcpy(string_command, temp);
  strcpy(string_echo, context);
  free(input_copy);

  if (strcmp(string_command, "pwd") == 0) {
    *arg_id = 1;
  }
  else if (strcmp(string_command, "time") == 0) {
    *arg_id = 2;
  }
  else if (strcmp(string_command, "clear") == 0) {
    *arg_id = 3;
  }
  else if (strcmp(string_command, "say") == 0) {
    *arg_id = 4;
  }
  else if (strcmp(string_command, "exit") == 0) {
    *arg_id = 7;
  }

  return 1;
}

void command_handler(int arg_id, char *string_input) {
  /*
  ARGS ID values

  1 = pwd // print current working directory
  2 = time // print current time
  3 = clear // clear console screen
  4 = say // echo string input
  */

  char cwd[PATH_MAX];

  if (arg_id == 1) {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("Current working dir: %s\n", cwd);
    } else {
      perror("getcwd() error");
    }
  }
  else if (arg_id == 2) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );
  }
  else if (arg_id == 3) {
    system("clear");
  }
  else if (arg_id == 4) {
    printf("%s\n", string_input);
  }
}

int main() {
  char string_input[120] = "Hello World";
  char string_command[120] = "Hello World";
  char string_echo[120] = "Hello World";

  int execution_flag = 0;
  int arg_id = 0;

  while (1) {
    // take input
    printf("MyOS> ");
    if (get_input(string_input))
      continue;
    // break;

    execution_flag = parse_input(string_input, string_command, string_echo, &arg_id);
    command_handler(arg_id, string_echo);

    if (arg_id == 7) {
      return 0;
    }
  }

  return 0;
}
