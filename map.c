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
      if (n <= 11)
      { *(mat -> data + (i * mat -> cols + j)) = _WALLU; }
      else if (n <= 29)
      { *(mat -> data + (i * mat -> cols + j)) = _WALLE; }
      else
      { *(mat -> data + (i * mat -> cols + j)) = _BGN; }
    }
  }

  // Around the player 1 to begin
  *(mat -> data) = _PLAYER;
  *(mat -> data + 1) = _BGN;
  *(mat -> data + (1 * mat -> cols + 0)) = _BGN;
  *(mat -> data + (1 * mat -> cols + 1)) = _BGN;

  // Around the player 2 to begin
  *(mat -> data + ((mat -> lines - 2) * mat -> cols + (mat -> cols - 2))) = _BGN;
  *(mat -> data + ((mat -> lines - 2) * mat -> cols + (mat -> cols - 1))) = _BGN;
  *(mat -> data + ((mat -> lines - 1) * mat -> cols + (mat -> cols - 2))) = _BGN;
  mat->data[(mat->lines - 1) * mat->cols + mat->cols - 1] = _PLAYER2;
}

void randomMap(struct matrix *map)
{
  for (size_t i = 0; i < map -> lines - 3; ++i)
  {
    for (size_t j = 0; j < map -> cols - 3; ++j)
    {
      int generated = 0;
      for (size_t n = i; n < i + 3; ++n)
      {
        for (size_t m = j; m < j + 3; ++m)
        {
          if (*(map -> data + (n * map -> cols + m)) == _WALLU)
          { ++generated; }
        }
      }
      if (generated >= 5)
      {
        *(map -> data + (i * map -> cols + j)) = _BGN;
        *(map -> data + ((i + 1) * map -> cols + j + 1)) = _BGN;
        *(map -> data + ((i + 2) * map -> cols + j + 2)) = _BGN;
      }
    }
  }
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
          printf("%sB%s", _KRED, _KWHT);
          break;
        case _PLAYER:
          printf("%sP%s", _KBLU, _KWHT);
          break;
        case _PLAYER2:
          printf("%sS%s", _KYLW, _KWHT);
          break;
        case _WALLE:
          printf("%s\"", "\x1B[32m");
          printf("%s", _KWHT);
          break;
        case _WALLU:
          printf("#");
          break;
        case _KBOOM:
          printf("%s+%s", _KRED, _KWHT);
          break;
        case _EXT:
          printf("E");
          break;
        case _DIE:
          printf("%sD%s", _KRED, _KWHT);
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
