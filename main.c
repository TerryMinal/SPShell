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
  char * semicolon = ";" ;
  char * str_pointer; 
  //printf("command:");
  while( fgets(str,sizeof(str), stdin) ) {
    //printf("str: %s\n", str);
    //printf("exit: %s\n", exit);
    //printf("RESULT: %d", strcmp (exit, str)); 
    if (strcmp (exit, str) == 0 ) {
      printf("User exited!\n");
      break;
    }

    while (str_pointer) {
      if (strstr(str,semicolon) != 0) {
	str_pointer = strstr(str, semicolon);
	str_pointer++;
	printf("after semi: %s\n", str_pointer);
	parse_args(str); 
	parse_args(str_pointer);
    }
      
  
    printf("command passed: %s\n", str);
    char ** args = parse_args(str);
  }
  
  return 0; 
}
