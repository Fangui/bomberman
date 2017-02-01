# include "matrix.h"


void game(size_t lines, size_t cols)
{
//  int isAlive = 1;  
  struct matrix *mat = newMat(lines, cols);
  buildMap(mat);

  printMat(mat);
  freeMat(mat);

}

int main()
{
  size_t lines = 9;
//  printf("lines & cols ? : ");
//  scanf("%zu", &lines);
  game(lines, lines);
  return 0;
}
