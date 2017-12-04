# SPShell
Project 2: make  shell

### Features that we have implemented
  * parsing the line to separate command from arguments
  * fork() and executing command
  * waiting until exec'd program exits
  * exit command
  * cd command
  * separating and executing multiple commands separated by `;`
  * Redirecting stdout and stdin
  * Piping

<hr>

### Features that we tried to implement but didn't pan out

<hr>

### Bugs
  * redirecting stdout and stdin (> , <) only works for one call of redirection. I.e `ls > content` works but `ls > content > content2` does not.
  * `cd ~` does not go to root directory
  * made up commands do not execute but also do not return any errors

<hr>

### Files
  * main.c
  * main.h

<hr>

### Function headers
```C
// prints all elements of an array
  void print_args(char **arr);
  // separates a string based on delimteer and makes it into an array
  // returns an array
  char ** parse_args(char * line, char * delimiter );
  //removes leading and trailing spaces of a given command
  // assumes no spaces are in the string other than the beginning and the end
  char * trim_spaces(char *str);
  // trims all the leading and trailing spaces of a list of commands
  void arr_trim_spaces(char **arr);
  // handles case where user uses redirecting ">" and "<"
  // returns 0,1: 0 is False when there is no redirection, 1 is True and there is redirection
  int redirect(char *str);
  // given array of commands such as [ls -al, cd .., less main.c]
  // executes all the given commands
  // array can be obtained through parse_args(args, STR_SEMICOLON)
  void execute_args(char **args);
  // execute piping if any
  void execute_pipes(char **args);
```

<hr>
