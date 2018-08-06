/*
  calculate.c -- Impelementation for an RPN calculator
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
#include "parse.h"
#include "vector.h"


double calculate(VECTOR *tokens)
{
  STACK *stack = make_empty_vector();
  size_t ip;
  TOKEN *current;
  size_t max_ip = get_size(tokens);
  VECTOR *tmp_tokens = make_empty_vector();
  double tmp = 0;

  for (ip = 0; ip < max_ip; ip++)
    {
      current = (TOKEN *)get(tokens, ip);
      if (get_token_type(current) == VALUE)
        {
          push(stack, (void *)current);
        }
      if (get_token_type(current) == OPERATOR)
        switch (get_token_operator(current))
          {
          case ADD:
            tmp = 0;
            tmp += get_token_number((TOKEN *)pop(stack));
            tmp += get_token_number((TOKEN *)pop(stack));
            push(tmp_tokens, (TOKEN *)new_number_token(tmp));
            push(stack, peek(tmp_tokens));
            break;
          case SUBTRACT:
            tmp = 0;
            tmp -= get_token_number((TOKEN *)pop(stack));
            tmp += get_token_number((TOKEN *)pop(stack));
            push(tmp_tokens, (void *)new_number_token(tmp));
            push(stack, peek(tmp_tokens));
            break;
          case MULTIPLY:
            tmp = 1;
            tmp *= get_token_number((TOKEN *)pop(stack));
            tmp *= get_token_number((TOKEN *)pop(stack));
            push(tmp_tokens, (void *)new_number_token(tmp));
            push(stack, peek(tmp_tokens));
            break;
          case DIVIDE:
            tmp = 1;
            tmp /= get_token_number((TOKEN *)pop(stack));
            tmp *= get_token_number((TOKEN *)pop(stack));
            push(tmp_tokens, (void *)new_number_token(tmp));
            push(stack, peek(tmp_tokens));
            break;
          default:
            break; /* Do nothing */
          }
    }
  tmp = get_token_number((TOKEN *)peek(stack));
  while (!is_empty(tmp_tokens))
    free(pop(tmp_tokens));
  destroy_vector(stack);
  destroy_vector(tmp_tokens);
  return tmp;
}
