/*
PROJECT TITLE: A Multithreaded Terminal Emulator
COURSE: Introduction to Operating Systems
SECTION: S12
AUTHORS: Rafael Maderazo & Eugenio Pastoral
*/

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
#include <pthread.h>

// Declare global variables for ncruses
int max_x, max_y;
WINDOW* mainwindow;

/*
This function takes in a string variable where it will store user input.
*/
int get_input(char *str) {
  // Declare buffer
  char buf[120];

  // Get user input in the window
  scanw("%[^\n]%*c", buf);

  // If there is no input, return 0. Otherwisem return 1.
  if (strlen(buf) != 0) {
    strcpy(str, buf);
    return 0;
  } else {
    return 1;
  }
}

/*
This function parses a string input and stores each part on the string_command and string_echo variables.
The argument identifier is also set that corresponds to the command.
*/
int parse_input(char *string_input, char *string_command, char *string_echo, int *arg_id) {
  // Declare variables for slicing string input
  char delimiter[] = " ";
  char *remainder, *context, *temp;

  // Slice the input
  int length = strlen(string_input);
  char *input_copy = (char*) calloc(length + 1, sizeof(char));
  strncpy(input_copy, string_input, length);
  temp = strtok_r (input_copy, delimiter, &context);

  // Assign the sliced strings to string_command and string_echo
  strcpy(string_command, temp);
  strcpy(string_echo, context);
  free(input_copy);

  // Set the appropriate argument identifiers based on the command entered
  if (strcmp(string_command, "pwd") == 0)
  *arg_id = 1;
  else if (strcmp(string_command, "time") == 0)
  *arg_id = 2;
  else if (strcmp(string_command, "clear") == 0 || strcmp(string_command, "cls") == 0)
  *arg_id = 3;
  else if (strcmp(string_command, "say") == 0)
  *arg_id = 4;
  else if (strcmp(string_command, "dir") == 0)
  *arg_id = 5;
  else if (strcmp(string_command, "marquee") == 0)
  *arg_id = 6;
  else if (strcmp(string_command, "exit") == 0)
  *arg_id = 7;
  else
  *arg_id = -1;

  return 1;
}

/*
This function runs the command specified by the argument identifer.

Argument identifiers and commands

1 = pwd // print current working directory
2 = time // print current time
3 = clear // clear console scree
4 = say // echo string input
5 = dir // print directory
*/
void command_handler(int arg_id, char *string_input) {
  // Declare variable for current working directory
  char cwd[PATH_MAX];

  // Print current working directory
  if (arg_id == 1) {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printw("Current working dir: %s\n", cwd);
    } else {
      perror("getcwd() error");
    }
  }

  // Print current time
  else if (arg_id == 2) {
    time_t rawtime;
    struct tm * timeinfo;
    wrefresh(stdscr);
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printw ( "Current local time and date: %s", asctime (timeinfo) );
  }

  // Clear console
  else if (arg_id == 4) {
    wrefresh(stdscr);
    printw("%s\n", string_input);
  }

  // Print directory
  else if (arg_id == 5){
    wrefresh(stdscr);
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
}

/*
This function creates a marquee based on a specified string.
*/
void *marquee(void* string_input){
  // Declare variables for printing the text
  char text[120] = " ";
  char c;
  int text_length;

  // Declare variables for subwindow creation
  int i, curr_x, curr_y;

  // Add space at both ends of the string
  strcat(text, string_input);
  strcat(text, " ");

  // Get text length
  text_length = strlen(text);

  // Refresh main window
  wrefresh(stdscr);

  // Instantiate a subwindow based on the current cursor position
  getyx(stdscr, curr_y, curr_x);
  WINDOW* subwindow = subwin(mainwindow, 1, max_x, curr_y, 0);

  // Print OS name and refresh main window
  printw("\nMyOS> ");
  refresh();

  // Refresh subwindow to show changes
  wrefresh(subwindow);

  // Scroll the text on the screen infinitely from left to right
  while (1) {
    sleep(0.5);

    // Scroll to the right
    for (i = 0; i < (max_x - text_length); i++) {
      wprintw(subwindow, text);
      wmove(subwindow, 0, i);
      wrefresh(subwindow);
      usleep(50000);

    }

    // Scroll to the left
    for (i = (max_x - text_length); i > 0; i--) {
      wprintw(subwindow, text);
      wmove(subwindow, 0, i);
      wrefresh(subwindow);
      usleep(50000);
    }
  }
}

int main() {
  // Declare and initialize string variables with dummy data
  char string_input[120] = "Hello World";
  char string_command[120] = "Hello World";
  char string_echo[120] = "Hello World";

  // Declare and initialize variables for command handling and threading
  int execution_flag = 0;
  int arg_id = 0;
  pthread_t pids[100];
  int pid_counter = 0;

  // Initialize main window and get max width and height of the console
  initscr();
  getmaxyx(stdscr, max_y, max_x);

  // Instantiate main window based on the max height and width
  mainwindow = newwin(max_y, max_x, 0, 0);

  // Infinitely get input until the exit command is entered
  while (1) {
    // Print OS name and refresh main window
    printw("MyOS> ");
    wrefresh(stdscr);

    // Get input
    if (get_input(string_input))
    continue;

    // Parse the input and execute the command
    execution_flag = parse_input(string_input, string_command, string_echo, &arg_id);
    command_handler(arg_id, string_echo);

    // Cancel all threads and clear the screen when clear command is invoked
    if (arg_id == 3){
      // Clear screen
      wclear(stdscr);
      wclear(mainwindow);

      // Move cursor back to top left
      wmove(stdscr, 0, 0);
      wmove(mainwindow, 0, 0);

      // Refresh to reflect changes
      wrefresh(stdscr);
      wrefresh(mainwindow);

      // Cancel all threads
      for(int i = 0; i < pid_counter; i++)
      pthread_cancel(pids[i]);

      // Reset thread counter to 0
      pid_counter = 0;
    }

    // When the marquee command is entered, run the command in a new thread
    else if (arg_id == 6){
      // Declare variables for threading
      pthread_t thread;
      pthread_attr_t attrib;
      int res;

      // Create the thread to run the command
      res = pthread_attr_init(&attrib);
      res = pthread_attr_setdetachstate(&attrib, PTHREAD_CREATE_DETACHED);
      res = pthread_create(&thread, &attrib, marquee, &string_echo);

      // Detach the thread
      pthread_detach(res);

      // Copy the thread to the array and increment the counter
      pids[pid_counter] = thread;
      pid_counter++;
    }

    // Cancel all threads and close all ncurses windows when the exit command is entered
    else if (arg_id == 7) {
      // Cancel all threads
      for(int i = 0; i < pid_counter; i++)
      pthread_cancel(pids[i]);

      // Close ncurses windows
      endwin();
      return 0;
    }

    // If command is unknown, print error message
    else if (arg_id == -1){
      printw("Invalid command. Please try again.\n");
    }
  }

  return 0;
}
