/*
  parse.h -- Public interface for parser
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

#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "vector.h"

typedef struct token TOKEN;

enum token_type
  {
   OPERATOR, VALUE
  };

typedef enum token_type TOKEN_TYPE;

enum operator_type
  {
   ADD, SUBTRACT, MULTIPLY, DIVIDE, INVALID
  };

typedef enum operator_type OPERATOR_TYPE;


/* Token operations */
TOKEN * new_number_token(double number);
TOKEN * new_operator_token(OPERATOR_TYPE op);

TOKEN_TYPE get_token_type(TOKEN *t);
double get_token_number(TOKEN *t);
OPERATOR_TYPE get_token_operator(TOKEN *t);
int get_token_priority(TOKEN *t);

/* Add and Subtract are 1, Mul and Divide are 2 */

void destroy_token(TOKEN *t);


/* Parser operations */
VECTOR * parse_expression(FILE *f);
TOKEN * get_token_input(FILE *f);

int char_is_number(char c);
OPERATOR_TYPE get_operator(char *s);

#endif
