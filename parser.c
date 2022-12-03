/*
 * parser.c
 *
 * Code to parse and tokenize a command line
 *
 * Author: Niyomwungeri Parmenide ISHIMWE
 */

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

/*
 * Documented in .h file
 */
int read_word(const char *input, char *word, size_t word_len)
{
  assert(input);
  assert(word);

  // TODO: Your code here!!
  const char *inp = input;
  char *w = word;
  bool betweenQuoteS = false;

  // SKIP THROUGH ANY LEADING WHITESPACE
  while (isspace(*inp))
    inp++;

  while (*inp)
  {
    // TODO: Handle unterminated double quote
    if (*inp == '"')
    {
      
      // COUNT THE NUMBER OF QUOTES AND CHECK IF IT IS ODD OR EVEN
      int quotesNbr = 0;
      for (int i = 0; i < strlen(input); i++)
      {
        if (input[i - 1] != '\\' && input[i] == '"')
          quotesNbr++;
      }

      // IF IT IS ODD, THEN IT IS NOT TERMINATED
      if (quotesNbr % 2 != 0)
      {
        sprintf(word, "Unterminated quote");
        return -1;
      }
    }

    if (isspace(*inp) && !betweenQuoteS)
    {
      // we've found the end of the token, so return
      break;
    }
    else if (*inp == '"')
    {
      // handle double quote
      betweenQuoteS = !betweenQuoteS;
      inp++;
    }
    else if (*inp == '\\')
    {
      // handle escape character
      switch (*(inp + 1))
      {
      case 'n':
        *w++ = '\n';
        break;

      case 'r':
        *w++ = '\r';
        break;

      case 't':
        *w++ = '\t';
        break;

      case '\\':
        *w++ = '\\';
        break;

      case ' ':
        *w++ = ' ';
        break;

      case '$':
        *w++ = '$';
        break;

      case '"':
        *w++ = '\"';
        break;
        // TODO: Handle these cases

      default: // ILLIGAL ESCAPE SEQUENCE
        sprintf(word, "Illegal escape character: %c", *(inp + 1));
        return -1;
      }

      inp += 2;
    }
    else
    {
      // COPY THE CHARACTER TO THE WORD
      *w++ = *inp++;
    }

    // WHEN THE WORD IS TOO LONG, RETURN -1
    if (w >= word + word_len)
    {
      strcpy(word, "Word too long");
      return -1;
    }
  }

  *w = '\0';
  return inp - input;
}

/*
 * Documented in .h file
 */
int parse_input(const char *input, char *argv[], int argv_max)
{
  int argc = 0;
  memset(argv, 0, argv_max);

  assert(input);

  // TODO: Your code here!!
  int MAX_WORD_LEN = 20;
  const char *inp = input;
  char *thisWord;

  while (*inp)
  {
    // SKIP LEADING WHITESPACE
    while (isspace(*inp))
      inp++;

    // IF WE REACH THE END OF THE INPUT, THEN WE ARE DONE
    if (*inp == '\0')
      break;

    // ALLOCATE MEMORY FOR THE CURRENT WORD
    thisWord = (char *) malloc(sizeof(char) * (MAX_WORD_LEN + 1));

    // READ THE NEXT WORD FROM THE INPUT
    int currWordLength = read_word(inp, thisWord, MAX_WORD_LEN);

    // ADD THE WORD TO THE ARRAY OF ARGUMENTS - argv
    argv[argc++] = thisWord;

    // CHECK FOR UNTERMINATED QUOTE
    int count = 0;
    for (int i = 0; i < strlen(input); i++)
    {
      // COUNT THE NUMBER OF QUOTES IN THE INPUT
      if (input[i - 1] != '\\' && input[i] == '"')
        count++;
    }

    // IF THE COUNT IS ODD, THEN THERE IS AN UNTERMINATED QUOTE
    if (count % 2 != 0)
    {
      strcpy(argv[0], "Unterminated quote");
      return -1;
    }

    // FAILED THIS TEST CASE
    // IF THE NUMBER OF ARGS EXCEEDS THE MAXIMUM NUMBER OF ARGS, THEN RETURN -1
    if (argc >= argv_max)
    {
      argv[argc] = NULL;
      strcpy(argv[0], "Too many arguments");
      return -1;
    }

    // GO TO THE NEXT WORD IN THE INPUT
    inp += currWordLength;
  }

  // POINT argv TO NULL
  argv[argc] = NULL;

  // ON SUCCESS, RETURN THE NUMBER OF ARGUMENTS
  return argc;
}
