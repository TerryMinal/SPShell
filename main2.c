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

void remove_extra_spaces(char **args) {
  int i =0;
  char *p; 
  while ( *(args + i) != NULL) {
    p = strstr( *(args + i), " "); 
    while (p != NULL) {
      p = "";
      
    }
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
  char str[100];
  char ** args; 
  char * exit = "exit";
  char semicolon = ';' ;
  char * str_pointer;
  char * space = " ";
  int exit_stat = 0;
 
  //printf("command:");
  while( fgets(str,sizeof(str), stdin) ) {
    //Stripping input of the "\n"
    str[strlen(str)-1] = 0;

    redirect(str);
    
    //Exit "signal"
    if (strcmp (exit, str) == 0 ) {
      printf("User exited!\n");
      exit_stat = 1; 
      break;
    }

    //If there is a semicolon in the input: 
    if (strchr(str, semicolon) != 0 ) {
      int i = 0; 
      //printf("SEMICOLON!\n");
      
      args = parse_args(str, &semicolon);
      printf("Array after parsing with semicolons\n"); 
      print_args(args);
      printf("****************\n");
      
      while (args[i] != NULL) {
	args[i] = * parse_args(args[i], " ");
	i++;
      }
      
      printf("Array after parsing with blank space\n");
      print_args(args);
    } 

    //No semicolons detected in input: 
    else {
      args = parse_args(str,space);
      printf("No semicolons detected. Parsing input through white spaces\n");
      print_args(args);
    } 
  
  }
  return 0;
}




  
