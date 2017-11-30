#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
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

#define EXIT "exit"
#define CD "cd"
void execute_args(char **args) {
  char **arg;
  int status;
  int i = 0;
  while ( *(args+i) != NULL) {
    arg = parse_args( *(args+i), " ");
    print_args(arg);
    //checking for special cases exit and cd
    if (strcmp (EXIT, arg[0]) == 0 ) {
      printf("User exited!\n");
      exit(1);
    }
    if (strcmp (arg[0], CD) == 0) {
      chdir(arg[1]);
    }
    //finished checking for special cases
    int p = fork();
    if (p == 0) {
      execvp(arg[0], arg);
      exit(1);
    }
    else
      wait(&status);
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
    args = parse_args(str, ";");
    execute_args(args);
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
