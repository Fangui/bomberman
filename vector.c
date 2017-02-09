# include "vector.h"

struct vector *vector_make(int capacity)
{
  struct vector *vect = malloc(sizeof(struct vector));
  vect->capacity = capacity;
  vect->size = 0;
  vect->data = malloc(sizeof(struct Tuple) * capacity);
  return vect;
}

void vector_push_back(struct vector *vect, struct Tuple *x)
{
  while(vect->size >= vect->capacity)
  {
    vect->capacity *= 2;
    vect->data = realloc(vect->data, sizeof(struct Tuple) * vect->capacity);
  }
  vect->data[vect->size] = x;
  ++(vect->size);
}

void clearVect(struct vector *vect)
{
  for(int i = 0; i < vect->size; ++i)
    free(vect->data[i]);
  vect->size = 0;
}

void freeVect(struct vector *vect)
{
  for(int i = 0; i < vect->size; ++i)
    free(vect->data[i]);
  free(vect->data);
  free(vect);
}
