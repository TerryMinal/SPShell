#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_args(char **arr) {
  int i = 0;
  while ( *(arr + i) != NULL ) {
    printf("command: %s\n", arr[i]);
    i++;
  }
}

char ** parse_args(char * line, char * delimiter ) {
  char **args = malloc(100 * sizeof(char *));
  char *remaining = line;
  int i = 0;
  while (remaining != NULL) {
    args[i] = strsep(&remaining, delimiter);
    i++;
    // printf("%s\n", remaining);
  }
  return args;
}

int main() {
  char str[100]; //contains the passed commands
  char ** args; //array of commands after parsing
  char * exit = "exit\n";  
  char semicolon = ';'; 
  char * str_pointer;
  char * space = " "; 
  //printf("command:");
  while( fgets(str,sizeof(str), stdin) ) {
    //printf("str: %s\n", str);
    //printf("exit: %s\n", exit);
    //printf("RESULT: %d", strcmp (exit, str)); 
    if (strcmp (exit, str) == 0 ) {
      printf("User exited!\n");
      break;
    }
    // find first occurrence of the character semicolon
    if (strchr(str, semicolon) != 0 ) {
      int i = 0; 
      printf("SEMICOLON!"); 
      args = parse_args(str, &semicolon);
      while (args[i] != NULL) {
	printf("args[%d] = %s\n", i, args[i]); 
	args[i] = * parse_args(args[i], " ");
	i++;
      }
      
      
      
    }
   
      
    /* 
    while (str_pointer) {
      if (strstr(str,semicolon) != 0) {
	str_pointer = strstr(str, semicolon);
	str_pointer++;
	printf("after semi: %s\n", str_pointer);
	parse_args(str); 
	parse_args(str_pointer);
	} */ 
      
  
    args = parse_args(str,space);
    print_args(args);
  }
  
  return 0; 
}
