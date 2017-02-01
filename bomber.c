# include "matrix.h"

# include <termios.h>

void game(size_t lines, size_t cols)
{
  int isAlive = 1;  
  struct matrix *mat = newMat(lines, cols);
  buildMap(mat);

  size_t posX = 0, posY = 0;
  struct termios info;
  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);

  while(isAlive)
  {
    printMat(mat);
    char c = getchar();
    if(c == 's') 
    {
      if(posY + 1 < mat->lines && mat->data[(posY + 1) * mat->cols + posX]==0)
      {
        mat->data[posY * mat->cols + posX] = 0;
        ++posY;
        mat->data[posY * mat->cols + posX] = 42;
      }
    }
    else if(c == 'z')
    {
      if(posY != 0 && mat->data[(posY - 1) * mat->cols + posX] == 0)
      {
        mat->data[posY * mat->cols + posX] = 0;
        --posY;
        mat->data[posY * mat->cols + posX] = 42;
      }
    }
    else if(c == 'd')
    {
      if(posX + 1 < mat->cols && mat->data[posY * mat->cols + posX + 1] == 0)
      {
        mat->data[posY * mat->cols + posX] = 0;
        ++posX;
        mat->data[posY * mat->cols + posX] = 42;
      }
    }
    else if(c == 'q')
    {
      if(posX != 0 && mat->data[posY * mat->cols + posX - 1] == 0)
      {
        mat->data[posY * mat->cols + posX] = 0;
        --posX;
        mat->data[posY * mat->cols + posX] = 42;
      }
    }
    else
      isAlive = 0; 
    printf("\e[1;1H\e[2J");
  }
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
