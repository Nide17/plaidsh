/*
 * plaidsh.c
 *
 * A small shell
 *
 * Author: Howdy Pierce <howdy@cardinalpeak.com>
 * Co-Author: Niyomwungeri Parmenide ISHIMWE <parmenin@andrew.cmu.edu>
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
  // ONE ARG AND IS exit OR quit
  if ((argc == 1 && strcmp(argv[0], "exit") == 0) || (strcmp(argv[0], "quit") == 0))
    exit(0);

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
  // THERE IS ONE ARG AND IS author
  if (argc == 1 && strcmp(argv[0], "author") == 0)
  {
    printf("Niyomwungeri Parmenide Parmenide\n");
    return 0;
  }

  // FAILURE
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
  // THERE IS A DIRECTORY AND THE FIRST ARG IS cd
  if (argc == 2 && strcmp(argv[0], "cd") == 0)
  {
    // CHANGE DIRECTORY TO ARG 1
    if (chdir(argv[1]) == 0)
      return 0;
  }
  
  // cd COMMAND FAILED
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
  // THERE IS ONE ARG AND IS THE pwd
  if (argc == 1 && strcmp(argv[0], "pwd") == 0)
  {
    char currentDir[1024];
    getcwd(currentDir, sizeof(currentDir));
    
    // PRINT THE CURRENT WORKING DIRECTORY
    printf("%s \n", currentDir);
    return 0;
  }

  // IF ERROR, RETURN 1
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
  // FORKING THE PROCESS
  pid_t pid = fork();

  // IF PARENT PROCESS - EXECUTE IT
  if (pid == 0)
  {
    execvp(argv[0], argv);
    exit(0);
  }

  // IF CHILD PROCESS, WAIT FOR IT TO FINISH
  else if (pid > 0)
  {
    int status;
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
  }

  // IF ERROR, COMMAND FAILED - RETURN -1
  else
    return -1;
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
  // EXECUTING THE exit, quit COMMAND
  if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0)
    builtin_exit(argc, argv);

  // EXECUTING THE author COMMAND
  else if (strcmp(argv[0], "author") == 0)
    builtin_author(argc, argv);

  // EXECUTING cd COMMAND
  else if (strcmp(argv[0], "cd") == 0)
    builtin_cd(argc, argv);

  // EXECUTING THE pwd COMMAND
  else if (strcmp(argv[0], "pwd") == 0)
    builtin_pwd(argc, argv);
  
  else
    forkexec_external_cmd(argc, argv);
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
    
    // GETTING THE INPUT FROM THE USER
    input = readline(prompt);

    // SAVING THE INPUT TO HISTORY
    add_history(input);

    // IF NO INPUT, KEEP PROMPTING
    if (input == NULL)
      exit(0);

    if (*input == '\0')
      continue;


    // GETTING AND PARSING THE INPUT
    argc = parse_input(input, argv, MAX_ARGS);

    // ARGUMENTS ERROR
    if (argc == -1)
      printf(" Error: %s\n", argv[0]);

    // THE ARGUMENTS ARE OK - RUN THE COMMAND
    else
      for (int i = 0; i < argc; i++)
        execute_command(argc, argv);
  }
}

int main(int argc, char *argv[])
{
  mainloop();
  return 0;
}
