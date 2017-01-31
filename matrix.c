# include "matrix.h"

struct matrix *newMat(size_t lines, size_t cols)
{
  struct matrix *mat = malloc(sizeof(struct matrix));
  mat->data = calloc(cols * lines, sizeof(int));
  mat->lines = lines;
  mat->cols = cols;

  return mat;
}

struct matrix *cloneMat(struct matrix *mat)
{
  struct matrix *Clone = newMat(mat->lines, mat->cols);
  for(size_t i = 0; i < mat->lines; ++i)
    for(size_t j = 0; j < mat->cols; ++j)
      Clone->data[i * mat->cols + j] = mat->data[i * mat->cols + j];

  return Clone;
}


void freeMat(struct matrix *mat)
{
  free(mat->data);
  free(mat);
}

void printMat(struct matrix *mat)
{
  for(size_t i = 0; i < mat->cols + 2; ++i)
    printf("-");
  printf("\n");

  for(size_t i = 0; i < mat->lines; ++i)
  {
    printf("|");
    for(size_t j = 0; j < mat->cols; j++)
    {
      if(mat->data[i * mat->cols + j] == 42)
        printf("P");
      else if(mat->data[i * mat->cols + j] == 1)
        printf("|");
      else if(mat->data[i * mat->cols + j] == 2)
        printf("-");
      else if(mat->data[i * mat->cols + j] == 27)
        printf("B");
      else
        printf(" ");
    }
    printf("|\n");
  }
  for(size_t i = 0; i < mat->cols + 2; ++i)
    printf("-");
  printf("\n");
}
