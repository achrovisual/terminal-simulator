#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curses.h>

int max_x, max_y;
WINDOW* mainwindow;

int get_input(char *str) {
  char buf[120];

  scanw("%[^\n]%*c", buf);
  // printw("%s", buf);
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
  else if (strcmp(string_command, "dir") == 0) {
    *arg_id = 5;
  }
  else if (strcmp(string_command, "marquee") == 0) {
    *arg_id = 6;
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
      printw("Current working dir: %s\n", cwd);
    } else {
      perror("getcwd() error");
    }
  }
  else if (arg_id == 2) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printw ( "Current local time and date: %s", asctime (timeinfo) );
  }
  else if (arg_id == 3) {
    system("clear");
  }
  else if (arg_id == 4) {
    printw("%s\n", string_input);
  }
  else if (arg_id == 5){

    struct dirent *currDir;
    struct stat acc;

    DIR *dr = opendir(".");

    if (dr == NULL)
    {
        printw("Directory cannot be opened." );
    }

    while ((currDir = readdir(dr)) != NULL){
      stat(currDir->d_name, &acc);
      printw("Filename: %s\n", currDir->d_name);
      printw("File size: %d Bytes\n", acc.st_size);
      printw("Created on: %s\n\n", ctime(&acc.st_mtime));
    }
    closedir(dr);
   }
   else if (arg_id == 6) {

     char text[120] = " ";
     char c;
     int text_length;
     int i, curr_x, curr_y;

     strcat(text, string_input);
     strcat(text, " ");
     // Get text length
     text_length = strlen(text);

     // Initialize screen for ncurses
     getyx(stdscr, curr_y, curr_x);
     WINDOW* subwindow = subwin(mainwindow, 1, max_x, curr_y, 0);
     // Don't show cursor
     // Get terminal dimensions
     // Scroll text across the screen once
     wrefresh(subwindow);
     while (1) {
       for (i = 0; i < (max_x - text_length); i++) {
         wprintw(subwindow, text);
         wmove(subwindow, 0, i);
         wrefresh(subwindow);
         usleep(50000);
       }

       for (i = (max_x - text_length); i > 0; i--) {
         wprintw(subwindow, text);
         wmove(subwindow, 0, i);
         wrefresh(subwindow);
         usleep(50000);
       }
     }
     // Wait for a keypress before quitting
     getch();
     endwin();
   }
}

int main() {
  char string_input[120] = "Hello World";
  char string_command[120] = "Hello World";
  char string_echo[120] = "Hello World";

  int execution_flag = 0;
  int arg_id = 0;

  initscr();
  getmaxyx(stdscr, max_y, max_x);

  mainwindow = newwin(max_y, max_x, 0, 0);
  scrollok(stdscr, 1);

  while (1) {
    // take input
    printw("MyOS> ");
    refresh();
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
