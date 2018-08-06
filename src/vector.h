
/*
  vector.h -- Public interface for the vectors
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

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct vector VECTOR;

/* Constructors and Destructors */
VECTOR * make_empty_vector();
void destroy_vector(VECTOR *vector);

/* Information operations */
int is_empty(VECTOR *vector);
size_t get_size(VECTOR *vector);
size_t get_space(VECTOR *vector);

/* Stack Operations*/
void * peek(VECTOR *vector);
void * pop(VECTOR *vector);

void push(VECTOR *vector, void * data);

/* Generic Vector Operations */
void * get(VECTOR *vector, size_t index);
void set(VECTOR *vector, size_t index, void *value);

void resize(VECTOR *vector, size_t size);

/* Stack typedef for programmer clarity */

typedef VECTOR STACK;

#endif
