/*
  calc.c -- Infix Calculator
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
#include "calculate.h"
#include "vector.h"
#include "parse.h"

FILE *f = NULL;

/* Settings toggled by flags */
int dump_rpn_code = 0;
FILE *dump_rpn_file = NULL;

int no_calculate = 0;

void dump_to_file(VECTOR *ops, FILE *output)
{
  size_t ops_size = get_size(ops);
  size_t i;
  TOKEN *token;
  char *op_string;
  for (i = 0; i < ops_size; i++)
    {
      token = (TOKEN *)get(ops, i);
      if (get_token_type(token) == VALUE)
        fprintf(output, "%g ", get_token_number(token));
      else if (get_token_type(token) == OPERATOR)
        {
          switch (get_token_operator(token))
            {
            case ADD:
              op_string = "+";
              break;
            case SUBTRACT:
              op_string = "-";
              break;
            case MULTIPLY:
              op_string = "*";
              break;
            case DIVIDE:
              op_string = "/";
              break;
            default:
              op_string = "?";
              break;
            }
          fprintf(output, "%s\n", op_string);
        }
    }
}


int unwind_protect(void)
{
  int exit = 0;
  int status = 0;
  if (f != NULL)
    {
      status = fclose(f);
      if (status != 0)
        {
          fprintf(stderr, "Cannot close input file\n");
          exit = -1;
        }
    }
  if (dump_rpn_file != NULL)
    {
      status = fclose(dump_rpn_file);
      if (status != 0)
        {
          fprintf(stderr, "Did not write successfully to output file\n");
          exit = -1;
        }
    }
  return exit;
}



double calc_dispatch(FILE *f)
{
  VECTOR *ops = parse_expression(f);
  if (dump_rpn_code)
    {
      dump_to_file(ops, dump_rpn_file);
    }
  if (no_calculate)
    {
      while(!is_empty(ops))
        free(pop(ops));
      exit(unwind_protect());
    }
  double result = calculate(ops);
  while(!is_empty(ops))
    free(pop(ops));
  destroy_vector(ops);
  return result;
}

int main(int argc, char **argv)
{
  double result = 0;
  int i;
  for (i = 1; i < argc; i++)
    {
      /* -c - Compile only */
      if (strcmp(argv[i], "-c") == 0)
        {
          no_calculate = 1;
          dump_rpn_code = 1;
          dump_rpn_file = stdin;
        }
      /* -d - Dump compiled code */
      else if (strcmp(argv[i], "-d") == 0)
        {
          dump_rpn_code = 1;
          if (argc > (i + 1))
            {
              i++;
              dump_rpn_file = fopen(argv[i], "w");
              if (dump_rpn_file == NULL)
                {
                  fprintf(stderr, "Cannot open file for writing: %s\n", argv[i]);
                  exit(unwind_protect());
                }
            }
          else
            {
              dump_rpn_file = stdout;
            }
        }
      else if (strcmp(argv[i], "-h") == 0)
        {
          printf("calc -- Simple infix calculator\n\
Written by Cody Rivera\n\
Copyright 2018 Cody Rivera\n\
This software is distributed under the terms of the GNU\n\
General Public License, version 3 or later\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details\n\
\n\
Calculates the infix expression in a particular [file]\n\n\
Usage : [file] [options]\n\
[options]\n\
-h : prints this screen\n\
-c : Compiles this file only\n\
-d [dump_file] : dumps to stdout or a file\n\n\
INFO: TOKENS MUST BE SEPARATED BY SPACES\n\n");
          exit(unwind_protect());
        }
      else
        {
          f = fopen(argv[i], "r");
          if (f == NULL)
            {
              fprintf(stderr, "Cannot open file: %s\n", argv[i]);
              exit(unwind_protect());
            }
          printf("Running file %s ...\n", argv[i]);
        }
    }
  if (f == NULL)
    {
      f = stdin;
      printf("Type expression, then type CTRL-D twice:\n");
    }
  result = calc_dispatch(f);
  printf("\nThe result is: %g\n", result);
  exit(unwind_protect());
}
