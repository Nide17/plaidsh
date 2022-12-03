## Plaid Shell(plaidsh) - Code for Assignment 4 for ILSE, 04-801 B2, Fall 2022
    
__INTRODUCTION__

This repo contains a program caled “Plaid Shell” which is a usable Linux shell. The program is a simple shell that can execute commands such as "pwd, author, exit, quit, and cd". The program is written in C and uses the GNU readline library for command line editing. The program is tested on Ubuntu 20.04.3 LTS and ScottyOne (Linux server installed on AWS).

__DESCRIPTION__
    
 An implementation of plaidsh for ILSE Assignment 4:
 
 Plaid Shell uses the following functions to work as a shell:
 1. int read_word(const char *input, char *word, int word_len)
    - Returns the first word from input, removing leading whitespace, handling double quotes, and translating escaped characters.

 2. int parse_input(const char *input, char *argv[], int argv_max)
    - Parses an input line into an argv vector by segmenting the input into words that are bounded by unquoted whitespace. In addition, double quotes are used to group words and are eliminated from the input.

 3. int builtin_exit(int argc, char *argv[]);
    - Handles the exit or quit commands, by exiting the shell.

 4. int builtin_author(int argc, char *argv[]);
    - Handles the author command, by printing the author's name.

 5. int builtin_cd(int argc, char *argv[]);
    - Handles the cd command, by changing the current working directory.

 6. int builtin_pwd(int argc, char *argv[]);
    - Handles the pwd command, by printing the current working directory.

 7. void execute_command(int argc, char *argv[]);
    - Executes a command by forking a child process and executing the command in the child process.
    - From within execute_command, it compares argv[0] to a number of constant strings, and call the appropriate builtin_XXXX function based on the result.

 8. int forkexec_external_cmd(int argc, char *argv[]);
    - Forks a child process and executes the command in the child process.
    - For the case where argv[0] does not match any of the builtin command names.

The plaidsh's main() function calls readline() in a loop. Each time readline returns, it prints the result via printf and the left and right arrow keys work and the tab completion of filenames works as well. In addition, the up and down arrow keys works by calling the add_history function from the readline library.


__IMPORTANCE__

Someone may need to use this program if they want to use a shell that can execute commands such as "pwd, author, exit, quit, and cd" and for exploration of the GNU readline library.

__GETTING STARTED__

- Clone this repository.
- Run the make command from its containing directory to get the better of it.
- Run the plaidsh executable to start the shell.
- Run the make clean command to clean up the directory.
- Check if it has effects.
- Happy exploration!!
  


 __KEYWORDS__

<mark>ILSE</mark>     <mark>CMU</mark>     <mark>Assignment4</mark>     <mark>plaidsh</mark>     <mark>C Programming</mark>     <mark>Strings</mark> 



  __AUTHOR__
    
 Written by parmenin (Niyomwungeri Parmenide ISHIMWE) at CMU-Africa - MSIT 
    

    
 __DATE__
    
 December 5, 2022" 
