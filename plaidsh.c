/*
 * plaidsh.c
 *
 * A small shell
 *
 * Author: Howdy Pierce <howdy@cardinalpeak.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#include "parser.h"

#define MAX_ARGS 20

/*
 * Handles the exit or quit commands, by exiting the shell. Does not
 * return.
 *
 * Parameters:
 *   argc     The length of the argv vector
 *   argv[]   The argument vector
 */
static int
builtin_exit(int argc, char *argv[])
{
  // TODO!
  if((argc == 1 && strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "quit") == 0)){
    exit(0);
  }

  return 0;
}

/*
 * Handles the author command, by printing the author of this program
 * to stdout
 *
 * Parameters:
 *   argc     The length of the argv vector
 *   argv[]   The argument vector, which is ignored
 *
 * Returns:
 *   0 on success, 1 on failure
 */
static int builtin_author(int argc, char *argv[])
{
  // TODO!
  if(argc == 1 && strcmp(argv[0], "author") == 0){
    printf("Niyomwungeri Parmenide\n");
    return 0;
  }
  return 1;
}

/*
 * Handles the cd builtin, by setting cwd to argv[1], which must exist.
 *
 * Parameters:
 *   argc     The length of the argv vector
 *   argv[]   The argument vector, which must have either 1 or 2 arguments.
 *
 * Returns:
 *   0 on success, 1 on failure
 */
static int builtin_cd(int argc, char *argv[])
{
  // TODO!
  if(argc == 2 && strcmp(argv[0], "cd") == 0){
    if(chdir(argv[1]) == 0){
      return 0;
    }
  }
  return 1;
}


/*
 * Handles the pwd builtin, by printing the cwd to the supplied file
 * descriptor
 *
 * Parameters (which are all ignored):
 *   argc     The length of the argv vector
 *   argv[]   The argument vector
 *
 * Returns:
 *   Always returns 0, since it always succeeds
 */
static int builtin_pwd(int argc, char *argv[])
{
  // TODO!
  if(argc == 1 && strcmp(argv[0], "pwd") == 0){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s \n", cwd);
    return 0;
  }
  return 1;
}

/*
 * Process an external (non built-in) command, by forking and execing
 * a child process, and waiting for the child to terminate
 *
 * Parameters:
 *   argc      The length of the argv vector
 *   argv[]    The argument vector
 *
 * Returns:
 *   The child's exit value, or -1 on error
 */
static int forkexec_external_cmd(int argc, char *argv[])
{
  // TODO!
  pid_t pid = fork();
  if (pid == 0)
  {
    execvp(argv[0], argv);
    exit(0);
  }
  else if (pid > 0)
  {
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
  }
  else
  {
    return -1;
  }
}

/*
 * Parses one input line, and executes it
 *
 * Parameters:
 *   argc     The length of the argv vector, which must be >= 1
 *   argv[]   The argument vector
 */
void execute_command(int argc, char *argv[])
{
  assert(argc >= 1);

  // TODO!
  if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0)
  {
    builtin_exit(argc, argv);
  }
  else if (strcmp(argv[0], "author") == 0)
  {
    builtin_author(argc, argv);
  }
  else if (strcmp(argv[0], "cd") == 0)
  {
    builtin_cd(argc, argv);
  }
  else if (strcmp(argv[0], "pwd") == 0)
  {
    builtin_pwd(argc, argv);
  }
  else
  {
    forkexec_external_cmd(argc, argv);
  }
}

/*
 * The main loop for the shell.
 */
void mainloop()
{
  char *input = NULL;
  char *argv[MAX_ARGS];
  int argc = 0;

  fprintf(stdout, "Welcome to Plaid Shell!\n");
  const char *prompt = "#> ";

  while (1)
  {
    input = readline(prompt);
    if (input == NULL)
      exit(0);

    if (*input == '\0')
      continue;

    argc = parse_input(input, argv, MAX_ARGS);
    
    if (argc == -1)
    { // handle parsing error
      printf(" Error: %s\n", argv[0]);
      free(argv[0]);
    }
    else
    { // parse_input succeeded
      for (int i = 0; i < argc; i++)
      {
        printf(" ARG %d: %s\n", i, argv[i]);

        // instead of printing out the arguments, you pass argc and argv to a new function, perhaps named execute_command.From within execute_command, you would compare argv[0] to a number of constant strings, and call the appropriate builtin_XXXX function based on the result.
        // TODO!
        execute_command(argc, argv);
        free(argv[i]);
      }
    }
    // free all the malloc'd memory
    free(input);
    for (int i = 0; i < argc; i++)
    {
      free(argv[i]);
    }
  }
}

int main(int argc, char *argv[])
{
  mainloop();
  return 0;
}
