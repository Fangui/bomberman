# ifndef VECTOR_H
# define VECTOR_H

# include "player.h"

struct Tuple
{
  int t1, t2;
};

struct vector
{
  int capacity, size;
  struct Tuple **data;
};

struct vector *vector_make(int capacity);

void vector_push_back(struct vector *vect, struct Tuple *x);

void freeVect(struct vector *vect);

# endif
