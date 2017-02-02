# include "map.h"
# include <time.h>

struct matrix * newMat(size_t lines, size_t cols)
{
  struct matrix *mat = malloc(sizeof(struct matrix));
  mat -> data = calloc(cols * lines, sizeof(int));
  mat -> lines = lines;
  mat -> cols = cols;
  return mat;
}

void freeMat(struct matrix *mat)
{
  free(mat -> data);
  free(mat);
}

void buildMap(struct matrix *mat)
{
  srand(time(NULL));

  for (size_t i = 0; i < mat -> lines; ++i)
  {
    for (size_t j = 0; j < mat -> cols; ++j)
    { *(mat -> data + (i * mat -> cols + j)) = rand() % 42; }
  }

  // Around the player to begin
  *(mat -> data) = _PLAYER;
  *(mat -> data + 1) = _BGN;
  *(mat -> data + (1 * mat -> cols + 0)) = _BGN;
  *(mat -> data + (1 * mat -> cols + 1)) = _BGN;
}

void printMat(struct matrix *mat)
{
  printf(" ");
  for (size_t i = 0; i < mat -> lines; ++i)
  { printf("=="); }

  for (size_t i = 0; i < mat -> lines; ++i)
  {
    printf("\n|");
    for (size_t j = 0; j < mat -> cols; ++j)
    {
      int n = *(mat -> data + (i * mat -> cols + j));
      if (n == _PLAYER)
      { printf("P"); }
      else if (n == _BOMB)
      { printf("B"); }
      else if (0 <= n && n <= 13)
      { printf("#"); }
      else
      { printf(" "); }
    }
    printf("|");
  }
  printf("\n ");

  for (size_t i = 0; i < mat -> lines; ++i)
  { printf("=="); }
  printf("\n");
}
