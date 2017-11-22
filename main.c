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
  char str[20]; 
  printf("command:");
  scanf("%s", str);
  printf("command passed: %s\n", str);
  return 0; 
}
