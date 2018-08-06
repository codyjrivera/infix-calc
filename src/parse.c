

/*
  parse.c -- Implementation of the parser
  Copyright (C) 2018 Cody Rivera

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "vector.h"

struct token
{
  union
  {
    OPERATOR_TYPE operator;
    double number;
  };
  TOKEN_TYPE type;
};


/* Token Operations */
TOKEN * new_number_token(double number)
{
  TOKEN *t = (TOKEN *)malloc(sizeof(TOKEN));
  if (t == NULL)
    {
      fprintf(stderr, "\nOut of Memory\n");
    }
  t->number = number;
  t->type = VALUE;
  return t;
}

TOKEN * new_operator_token(OPERATOR_TYPE op)
{
  TOKEN *t = (TOKEN *)malloc(sizeof(TOKEN));
  if (t == NULL)
    {
      fprintf(stderr, "\nOut of Memory\n");
    }
  t->operator = op;
  t->type = OPERATOR;
  return t;
}

TOKEN_TYPE get_token_type(TOKEN *t)
{
  return t->type;
}

double get_token_number(TOKEN *t)
{
  return t->number;
}

OPERATOR_TYPE get_token_operator(TOKEN *t)
{
  return t->operator;
}

int get_token_priority(TOKEN *t)
{
  if ((get_token_operator(t) == ADD) || (get_token_operator(t) == SUBTRACT))
    return 1;
  else
    return 2;
}

void destroy_token(TOKEN *t)
{
  free(t);
}


/*
   The parser: parses from a FILE * until EOF

   Tokens:  Separated by whitespace
   Anything beginning with 0-9 will be converted into a number
   by ftoi.
   +, -, *, and / will be parsed into their respective operator
   tokens.

   Parsing: Uses Djkstra's Shuffling Yard Algorithm to convert
   infix notation to prefix notation.

   Errors: Message printed to stderr, parser returns null pointer.
*/


TOKEN * get_token_input(FILE *f)
{
  /* Local Variables used for parsing */
  char *token_string = (char *)malloc(32 * sizeof(char));
  size_t string_length = 0;
  size_t string_space = 32;
  char ch;
  /* Local Variables used for constructing token */
  TOKEN *token = NULL;
  TOKEN_TYPE type;
  double value;
  OPERATOR_TYPE operator;
  ch = fgetc(f);
  if (feof(f))
    {
      free(token_string);
      return NULL;
    }
  /* Following loop advances to next valid character */
  while ((ch == ' ') || (ch == '\n') || (ch == '\r'))
    {
      ch = fgetc(f);
      if (feof(f))
        {
          free(token_string);
          return NULL;
        }
    }
  if (char_is_number(ch))
    type = VALUE;
  else
    type = OPERATOR;
  token_string[string_length] = ch;
  for (string_length = 1;; string_length++)
    {
      ch = fgetc(f);
      if (feof(f) || (ch == ' ') || (ch == '\n') || (ch == '\r'))
        {
          break;
        }
      if (string_length >= string_space - 1)
        {
          string_space++;
          token_string = (char *)realloc(token_string, string_space * sizeof(char));
        }
      token_string[string_length] = ch;
    }
  token_string[string_length] = '\0';
  if (type == VALUE)
    {
      value = atof(token_string);
      token = new_number_token(value);
    }
  if (type == OPERATOR)
    {
      operator = get_operator(token_string);
      if (operator == INVALID)
        {
          fprintf(stderr, "\nInvalid Operator: %s\n", token_string);
          token = NULL;
        }
      else
        token = new_operator_token(operator);
    }
  free(token_string);
  return token;
}

int char_is_number(char c)
{
  return ((c >= '0') && (c <= '9'));
}

OPERATOR_TYPE get_operator(char *s)
{
  if (strcmp(s, "+") == 0)
    return ADD;
  else if (strcmp(s, "-") == 0)
    return SUBTRACT;
  else if (strcmp(s, "*") == 0)
    return MULTIPLY;
  else if (strcmp(s, "/") == 0)
    return DIVIDE;
  else
    return INVALID;
}


VECTOR * parse_expression(FILE *f)
{
  STACK *op_stack = make_empty_vector();
  VECTOR *output_stack = make_empty_vector();
  TOKEN *token = NULL;
  /* Parsing loop */
  while (!feof(f))
    {
      token = get_token_input(f);
      if (token == NULL)
        {
          break;
        }
      if (get_token_type(token) == OPERATOR)
        {
          while (!is_empty(op_stack) &&
                 get_token_priority((TOKEN *)peek(op_stack)) >=
                 get_token_priority(token))
            {
              push(output_stack, pop(op_stack));
            }
          push(op_stack, (void *)token);
        }
      if (get_token_type(token) == VALUE)
        {
          push(output_stack, (void *)token);
        }
    }
  while (!is_empty(op_stack))
    {
      push(output_stack, pop(op_stack));
    }
  destroy_vector(op_stack);
  return output_stack;
}
