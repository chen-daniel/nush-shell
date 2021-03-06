/* This file is from Spring 2019 Assignment 5 CS3650 */
/* Author: Daniel Chen */

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "svec.h"

// Read in an operator that is a single char
int read_single_op(svec *xs, const char *text, int *ii, char op)
{
  if (text[(*ii)] == op)
  {
    char *sop = calloc(2, sizeof(char));
    sop[0] = op;
    sop[1] = 0;

    // Push op to xs
    svec_push_back(xs, sop);
    free(sop);
    (*ii)++;
    return 1;
  }
  return 0;
}

// Read in an operator that may be two chars
int read_double_op(svec *xs, const char *text, int *ii, int nn, char op)
{
  if (text[(*ii)] == op)
  {
    if ((*ii) + 1 < nn && text[(*ii) + 1] == op)
    {
      char *sop = calloc(3, sizeof(char));
      sop[0] = op;
      sop[1] = op;
      sop[2] = 0;

      // Push op to xs
      svec_push_back(xs, sop);
      free(sop);
      (*ii) += 2;
      return 1;
    }
    else
    {
      char *sop = calloc(2, sizeof(char));
      sop[0] = op;
      sop[1] = 0;

      // Push op to xs
      svec_push_back(xs, sop);
      free(sop);
      (*ii)++;
      return 1;
    }
  }
  return 0;
}

// Read in any operator
int read_ops(svec *xs, const char *text, int *ii, int nn)
{
  return read_single_op(xs, text, ii, '<') ||
         read_single_op(xs, text, ii, '>') ||
         read_single_op(xs, text, ii, ';') ||
         read_double_op(xs, text, ii, nn, '|') ||
         read_double_op(xs, text, ii, nn, '&');
}

// Check if ch is an operator character
int isop(char ch)
{
  return ch == '<' ||
         ch == '>' ||
         ch == ';' ||
         ch == '|' ||
         ch == '&';
}

// Read in the next token
void read_token(svec *xs, const char *text, int *ii, int nn)
{
  int start = (*ii);

  // Add a flag for if there is a quote
  int quote = text[start] == '\'' || text[start] == '"';

  if (quote)
  {
    (*ii)++;
  }

  int paren = text[start] == '(';
  int parenCount = 0;
  int quoteStart = 0;
  if (paren)
  {
    parenCount++;
    (*ii)++;
  }

  // If there is a quote, wait until the end quote is reached
  // Else wait until there is a space or operator char
  while ((*ii) < nn)
  {
    char curr = text[(*ii)];
    if (!quote && !paren)
    {
      if (isspace(curr) || isop(curr))
      {
        break;
      }
    }
    else if (quote && curr == text[start])
    {
      break;
    }
    else if (paren)
    {
      if (curr == '"' || curr == '\'')
      {
        if (quoteStart)
        {
          quoteStart = 0;
        }
        else
        {
          quoteStart = (*ii);
        }
      }
      else if (!quoteStart && curr == '(')
      {
        parenCount++;
      }
      else if (!quoteStart && curr == ')')
      {
        parenCount--;
        if (parenCount == 0)
        {
          break;
        }
      }
    }
    (*ii)++;
  }
  // If there were quotes, exclude the beginning quote
  if (quote)
  {
    start++;
  }
  // If there were parens, include the end paren
  if (paren)
  {
    (*ii)++;
  }

  // Push token to xs
  int len = (*ii) - start;

  char *token = calloc((len + 1), sizeof(char));
  memcpy(token, text + start, len);
  token[len] = 0;
  svec_push_back(xs, token);
  free(token);

  // If there were quotes or parens, skip past the endquote
  if (quote)
  {
    (*ii)++;
  }
}

// Tokenize the line into an svec
svec *tokenize(const char *text)
{
  svec *xs = make_svec();

  int nn = strlen(text);
  int ii = 0;

  // Iterate through text
  while (ii < nn)
  {
    // If next char is a space, skip
    if (isspace(text[ii]))
    {
      ii++;
      continue;
    }

    // If next char is an operator, read it and continue loop
    if (read_ops(xs, text, &ii, nn))
    {
      continue;
    }

    // Read in a token
    read_token(xs, text, &ii, nn);
  }

  return xs;
}

// Tokenize a paren expression into an svec
svec *tokenizeParen(const char *text)
{
  svec *xs = make_svec();

  int nn = strlen(text) - 1;
  int ii = 1;

  // Iterate through text
  while (ii < nn)
  {
    // If next char is a space, skip
    if (isspace(text[ii]))
    {
      ii++;
      continue;
    }

    // If next char is an operator, read it and continue loop
    if (read_ops(xs, text, &ii, nn))
    {
      continue;
    }

    // Read in a token
    read_token(xs, text, &ii, nn);
  }

  return xs;
}
