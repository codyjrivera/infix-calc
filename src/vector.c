
/*
  vector.c -- Implementation of the vector
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
#include "vector.h"

/* Vector Structure */

struct vector
{
  void **contents;
  size_t size; /* Number of elements on the vector */
  size_t space; /* Amount of void * slots allocated */
};

/* Constructors and Destructors */

VECTOR * make_empty_vector()
{
  VECTOR *vector = (VECTOR *)malloc(sizeof(VECTOR));
  if (vector == NULL)
    {
      fprintf(stderr, "Out of Memory\n");
      exit(-1);
    }
  vector->contents = (void **)malloc(10 * sizeof(void *));
  vector->size = 0;
  vector->space = 0;
  return vector;
}

/* Destroys vector (but NOT its contents) */
void destroy_vector(VECTOR *vector)
{
  free(vector->contents);
  free(vector);
}


/* Basic Vector Operations */
int is_empty (VECTOR *vector)
{
  return vector->size == 0;
}

size_t get_size(VECTOR *vector)
{
  return vector->size;
}

size_t get_space(VECTOR *vector)
{
  return vector->space;
}

void * peek(VECTOR *vector)
{
  if (is_empty(vector))
    return NULL;
  else
    return vector->contents[vector->size - 1];
}

/* Only deallocates memory when difference between vector size and memory
   space is greater than 10 */
void * pop(VECTOR *vector)
{
  void *value;
  if (is_empty(vector))
    return NULL;
  else
    {
      value = vector->contents[vector->size - 1];
      vector->size--;
      if ((vector->space - vector->size) > 10)
        {
          vector->space -= 10;
          vector->contents = (void **)realloc(vector->contents,
                                              vector->space * sizeof(void *));
          if (vector == NULL)
            {
              fprintf(stderr, "Memory Error\n");
              exit(-1);
            }
        }
      return value;
    }
}

/* When there is no more memory left, allocates 10 more slots  */
void push(VECTOR *vector, void *value)
{
  vector->size++;
  if (vector->size >= vector->space)
    {
      vector->space += 10;
      vector->contents = (void **)realloc(vector->contents,
                                          vector->space * sizeof(void *));
      if (vector == NULL)
        {
          fprintf(stderr, "Out of Memory\n");
          exit(-1);
        }

    }
  vector->contents[vector->size - 1] = value;
}


void * get(VECTOR *vector, size_t index)
{
  if ((vector->size == 0) || (index > vector->size - 1))
    return NULL;
  else
    return vector->contents[index];
}

void set(VECTOR *vector, size_t index, void *value)
{
  if ((vector->size == 0) || (index > vector->size - 1))
    return; /* Do nothing */
  else
    vector->contents[index] = value;
}



void resize(VECTOR *vector, size_t size)
{
  if (size <= 10) /* 10 is minimum size */
    vector->contents = (void **)realloc(vector->contents, 10 * sizeof(void *));
  else
    {
      /* Rounds the size up to the nearest 10; for example, 11-19 would yield
         a size of 20. Done using size mod 10 to determine the ones, subtracting
         that from 10, adding that to the size, and assigning vector->size to that
         value */
      vector->space = size + (10 - (size % 10));
      vector->contents = (void **)realloc(vector->contents,
                                          vector->space * sizeof(void *));
      if (vector == NULL)
        {
          fprintf(stderr, "Memory Error\n");
          exit(-1);
        }
    }
}
