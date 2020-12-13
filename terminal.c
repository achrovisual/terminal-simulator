#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int main() {
  char cwd[PATH_MAX];
  time_t rawtime;
  struct tm * timeinfo;

  char string_input[120] = "Hello World";

  int arg_id = 4;

  /*
  ARGS ID values

  1 = pwd // print current working directory
  2 = time // print current time
  3 = clear // clear console screen
  4 = say // echo string input
  */

  if (arg_id == 1) {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("Current working dir: %s\n", cwd);
    } else {
      perror("getcwd() error");
      return 1;
    }
  }
  else if (arg_id == 2) {
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s", asctime (timeinfo) );
  }
  else if (arg_id == 3) {
    system("cls");
  }
  else if (arg_id == 4) {
    printf("%s\n", string_input);
  }
  return 0;
}
