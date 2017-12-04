// Terry Guan Queenie Xiang
// PD5
// System Programming Shell

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "main.h"

#define STR_SPACE " "
#define STR_SEMICOLON ";"

// prints all elements of an array
void print_args(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    printf("command: %s\n", arr[i]);
    i++;
  }
}

// separates a string based on delimteer and makes it into an array
// returns an array
char ** parse_args(char * line, char * delimiter ) {
  char **args = calloc(100, sizeof(char *));
  int i = 0;
  char *comm;
  while (line != NULL) {
    comm = strsep(&line, delimiter);
    if (strlen(comm) != 0) { //adds to array only if it is not an empty string
      args[i] = comm;
      i++;
    }
  }
  args[i] = NULL;
  return args;
}

//removes leading and trailing spaces of a given command
// assumes no spaces are in the string other than the beginning and the end
char * trim_spaces(char *str) {
  char **ret = parse_args(str, STR_SPACE);
  return ret[0];
}

// trims all the leading and trailing spaces of a list of commands
void arr_trim_spaces(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    *(arr + i) = trim_spaces(*(arr + i));
    i++;
  }
}


// handles case where user uses redirecting ">" and "<"
// returns 0,1: 0 is False, 1 is True
int redirect(char *str) {
  char **args; // holds an array with commands before and after  ">"/"<"
  char **command; // holds
  int fd; //holds the file descriptor of the desired file to redirect to
  int f; // holds the file descriptor of stdin or stdout
  int status; // holds exit status
  if (strstr( str, ">") != NULL) { //case where user is redirecting stdout
    args = parse_args( str, ">"); // parses to separate something like "command>file" into [command, file]
    arr_trim_spaces(args);
    // print_args(args);
    //creates a file with name passed in args if none exist, opens in write only and deletes any previous content otherwise
    fd = open(args[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    f = STDOUT_FILENO;
    // printf("stdout\n");
  }
  else if (strstr( str, "<") != NULL) { //case where user is redirecting stdin
    args = parse_args( str, "<");
    arr_trim_spaces(args);
    // print_args(args);
    // creates a file for read only
    fd = open(args[1], O_RDONLY, 0666);
    f = STDIN_FILENO;
    // printf("stdin\n");
  }
  else { //if the user didn't attempt to redirect
    // printf("did not redirect\n");
    return 0;
  }
  command = parse_args(args[0], STR_SPACE);
  int p = fork(); // fork child to execute redirection
  if (p == 0) { // if child
    int cp = dup(f); //make copy of stdin/stdout
    dup2(fd, f); // duplicate behavior of stdin/stdout to file fd
    close(fd); // close file fd
    execvp(command[0], command); //executes command. Command puts output into stdout/stdin. Dup2 duplicates that behavior
    dup2(cp, f); //reinstate the original std file by copying the stdin/stdout copy back into its original place
    close(cp); //close copy of stdout/stdin
    exit(1); //end child
  }
  else {
    wait(&status);
    return 1;
  }
}

// execute piping if any
void execute_pipes(char **args) {
  char **comm2 = parse_args(args[1], STR_SPACE);
  int status;
  FILE *fp = popen(args[0], "r"); // run the first command
  if (fp == NULL) {
    printf("Error creating pipe!\n");
  }
  else {
    int p = fork();
    if (p == 0) {
      int cp = dup(STDIN_FILENO); // make duplicate of stdin
      dup2(fileno(fp), STDIN_FILENO); // reads fp and stdin duplicates it
      // print_args(comm2);
      execvp(comm2[0], comm2);
      dup2(cp, STDIN_FILENO); // reinstates stdin at original file descriptor
      pclose(fp);
    }
    else {
      wait(&status);
    }
  }
}



// given array of commands such as [ls -al, cd .., less main.c]
// executes all the given commands
// array can be obtained through parse_args(args, STR_SEMICOLON)
void execute_args(char **args) {
  char *EXIT = "exit";
  char *CD = "cd";
  char * PIPE = "|";
  char **arg; //array of single command
  int status; // status of exit
  int i = 0; // incrementor for array
  int p; // used for forking

  while ( *(args+i) != NULL) { // loops through all the commands
    if (redirect(*args+i)) { // check whether if it's redirection
      // printf("redirected...\n" );
    }
    else if (strstr( *(args + i), PIPE) != NULL) { // if it is piping
      arg = parse_args( *(args + i) , PIPE);
      execute_pipes(arg);
    }
    else { // if it's not redirection it must be a normal command with args
      arg = parse_args( *(args+i), STR_SPACE); //splits into command and args in non redirectional commands
      // print_args(arg);
      // trim_spaces(arg[0]);
      //checking for special cases exit and cd

      if (strcmp (EXIT, arg[0]) == 0 ) { // checking for exit command case
        printf("User exited!\n");
        exit(1);
      }
      else if (strcmp (arg[0], CD) == 0) { // checking for cd command case
        chdir(arg[1]); // changes current directory
      }

      //finished checking for special cases
      else {
        p = fork();
        if (p == 0) { // child process
          // trim_spaces(arg[1]);
          execvp(arg[0], arg);
          exit(1); //quits the child process
        }
        else
          wait(&status);
      }
    }
    i++;
  } // end while
  // after function runs all commands have been run
}


int main() {
  char str[100];
  char ** args;
  char * exit = "exit";

  while( fgets(str,sizeof(str), stdin) ) {
    str[strlen(str)-1] = 0; //Stripping input of the "\n"
    args = parse_args(str, STR_SEMICOLON); //separates the commands. Will provide one command when there is only one command
    execute_args(args); //run all the commands obtained above
  }
  return 0;
}
