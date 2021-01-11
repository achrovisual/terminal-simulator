# A Multithreaded Terminal Emulator
A simple multi-threaded terminal simulator for our course in operating systems.

## How to Compile
Since this ncurses and POSIX threads were used, additional arguments should be entered when compiling the code. Please refer to the example below.
```
cc terminal.c -lpthread -lncurses
```
To run the program, just open the binary executable produced in terminal.

## Commands
| Command  | Description |
| ------------- | ------------- |
| say <string> | Displays the string in a new line. The string can have spaces in between words.  |
| cls  | Clears the entire screen.  |
| dir  | Displays the content of the current directory. This includes the filename, filesize, date and time created of each file.  |
| pwd  | Prints current working directory (ex: C:\Users\student\).  |
| time  | Prints the current date and time based on the clock.  |
| marquee <string>  | Displays a scrolling string that will move from left to right. While the string is animating from left to right, you must still be able to enter and execute commands in the terminal.   |
| exit  | Terminates the program and closes the window.  |
