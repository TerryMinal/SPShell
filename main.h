

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
