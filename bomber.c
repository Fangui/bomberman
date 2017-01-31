# include "matrix.h"


void game(size_t lines, size_t cols)
{
  
  struct matrix *mat = newMat(lines, cols);
  srand(time(NULL));
  for(size_t i = 0; i < mat->lines; ++i)
    for(size_t j = 0; j < mat->cols; ++j)
      mat->data[i * mat->cols + j] = rand() % 8;

  printMat(mat);
  freeMat(mat);

}

int main()
{
  size_t lines;
  printf("lines & cols ? : ");
  scanf("%zu", &lines);
  game(lines, lines);
  return 0;
}
