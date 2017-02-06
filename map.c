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
  int n;

  for (size_t i = 0; i < mat -> lines; ++i)
  {
    for (size_t j = 0; j < mat -> cols; ++j)
    {
      n = rand() % 42;
      if (n <= 13)
      { *(mat -> data + (i * mat -> cols + j)) = _WALLU; }
      else if (n <= 27)
      { *(mat -> data + (i * mat -> cols + j)) = _WALLE; }
      else
      { *(mat -> data + (i * mat -> cols + j)) = _BGN; }
    }
  }

  // Around the player to begin
  *(mat -> data) = _PLAYER;
  *(mat -> data + 1) = _BGN;
  *(mat -> data + (1 * mat -> cols + 0)) = _BGN;
  *(mat -> data + (1 * mat -> cols + 1)) = _BGN;

  *(mat -> data + (1 * mat -> cols + 2)) = _BGN;
  *(mat -> data + (1 * mat -> cols + 3)) = _BGN;

  mat->data[(mat->lines - 1) * mat->cols + mat->cols - 1] = _PLAYER2;
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
      switch (n)
      {
        case _BGN:
          printf(" ");
          break;
        case _BOMB:
          printf("B");
          break;
        case _PLAYER:
          printf("P");
          break;
        case _PLAYER2:
          printf("2");
          break;
        case _WALLE:
          printf("\"");
          break;
        case _WALLU:
          printf("#");
          break;
        case _KBOOM:
          printf("+");
          break;
        default:
          break;
      }
    }
    printf("|");
  }
  printf("\n ");

  for (size_t i = 0; i < mat -> lines; ++i)
  { printf("=="); }
  printf("\n");
}
