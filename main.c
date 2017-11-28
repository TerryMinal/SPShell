#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char ** parse_args(char * line ) {
  char **args = malloc(6 * sizeof(char *));
  char *remaining = line;
  int i = 0;
  while (remaining != NULL) {
    args[i] = strsep(&remaining, " ");
    i++;
    // printf("%s\n", remaining);
  }
  return args;
}

int main() {
  char str[100];
  char * exit = "exit\n"; 
  //printf("command:");
  while( fgets(str,sizeof(str), stdin) ) {
    //printf("str: %s\n", str);
    //printf("exit: %s\n", exit);
    //printf("RESULT: %d", strcmp (exit, str)); 
    if (strcmp (exit, str) == 0 ) {
      printf("User exited!\n");
      break;
    }
  
    printf("command passed: %s\n", str);
    char ** args = parse_args(str);
  }
  
  return 0; 
}
