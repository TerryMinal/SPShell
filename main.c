#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// char * str_replace(char *str, char replace, char replace_with) {
//   char *n = strchr(str, '\n');
//   if (replace_with == NULL)
//     replace_with = '/0';
//   *n = replace_with; //removes the newline from fgets
//   return str;
// }

void print_args(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    printf("command: %s\n", arr[i]);
    i++;
  }
}

char ** parse_args(char * line, char * delimiter ) {
  char **args = calloc(100, sizeof(char *));
  int i = 0;
  while (line != NULL) {
    args[i] = strsep(&line, delimiter);
    i++;
    // printf("%s\n", remaining);
  }
  args[i] = NULL;
  return args;
}

int redirect(char *str) {
  char **args;
  char **command;
  int p;
  int fd;
  int f; // stdin or stdout
  int status;
  if (strstr( str, ">") != NULL) {
    args = parse_args( str, ">");
    print_args(args);
    fd = open(args[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    f = STDOUT_FILENO;
    printf("stdout\n");
  }
  else if (strstr( str, "<") != NULL) {
    args = parse_args( str, "<");
    print_args(args);
    fd = open(args[1], O_RDONLY, 0666);
    f = STDIN_FILENO;
    printf("stdin\n");
  }
  else {
    printf("did not redirect\n");
    return 0;
  }
  command = parse_args(args[0], " ");
  p = fork();
  if (p == 0) {
    int cp = dup(f);
    printf("forked\n");
    dup2(fd, f);
    close(fd);
    execvp(command[0], command);
    dup2(cp, f); //reinstate the original std file
    close(cp);
    exit(1);
  }
  else {
    wait(&status);
    return 1;
  }
}

#define EXIT "exit"
#define CD "cd"
void execute_args(char **args) {
  char **arg;
  int status;
  int i = 0;
  int p;
  while ( *(args+i) != NULL) { //loops through all the commands
    if (redirect(*args+i)) {
      printf("redirected...\n" );
    }
    else {
      arg = parse_args( *(args+i), " "); //splits into command and args in non redirectional commands
      print_args(arg);
      //checking for special cases exit and cd
      if (strcmp (EXIT, arg[0]) == 0 ) {
        printf("User exited!\n");
        exit(1);
      }
      else if (strcmp (arg[0], CD) == 0) {
        chdir(arg[1]);
      }
      //finished checking for special cases
      else {
        p = fork();
        if (p == 0) {
          execvp(arg[0], arg);
          exit(1); //quits the child process
        }
        else
          wait(&status);
      }
    }
    i++;
  }
}


int main() {

  char str[100]; //contains the passed commands
  char **args;
  char *n;
  int status;
  int i = 0;
  // printf("%s\n", strerror(errno));
  while( fgets(str, sizeof(str), stdin) ) {
    char *n = strchr(str, '\n');
    *n = '\0'; //removes the newline from fgets
    args = parse_args(str, ";"); //separates the commands. Will provide one command when there is only one command
    execute_args(args); //run all the commands obtained above
  }
  //   if (strchr(str, ';') != NULL) {
  //     args = parse_args(str, ";");
  //     execute_args(args);
  //   } //end if
  //   else {
  //     args = parse_args(str, " ");
  //   }
  // }

  return 0;
}
