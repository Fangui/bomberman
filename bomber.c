# define _XOPEN_SOURCE 500

# include "map.h"

# include <termios.h>
# include <unistd.h>

void kboom(struct matrix *mat, size_t lines, size_t cols, int field)
{
  int x = (int)cols;
  int y = (int)lines;
  int left = field;
  int right = field;
  int up = field;
  int down = field;

  mat->data[lines * mat->cols + cols] = _KBOOM;

  for (int i = x; i >= 0 && left >= 0 && mat->data[lines * mat->cols + i] != _WALLU; --i, --left)
  {
    if (mat->data[lines * mat->cols + i] == _WALLE)
      left = 0;
    mat->data[lines * mat->cols + i] = _KBOOM;
  }

  for (int j = y; j >= 0 && up >= 0 && mat->data[j * mat->cols + cols] != _WALLU; --j, --up)
  {
    if (mat->data[j * mat->cols + cols] == _WALLE)
      up = 0;
    mat->data[j * mat->cols + cols] = _KBOOM;
  }

  for (int i = x; i < (int)mat->cols && right >= 0 && mat->data[lines * mat->cols + i] != _WALLU; ++i, --right)
  {
    if (mat->data[lines * mat->cols + i] == _WALLE)
      right = 0;
    mat->data[lines * mat->cols + i] = _KBOOM;
  }

  for (int j = y; j < (int)mat->lines && down >= 0 && mat->data[j * mat->cols + cols] != _WALLU; ++j, --down)
  {
    
    if (mat->data[j * mat->cols + cols] == _WALLE)
      down = 0;
    mat->data[j * mat->cols + cols] = _KBOOM;
  }
}

void game(size_t lines, size_t cols)
{
  struct timespec start, end;
    
  int isAlive = 1, bomb = 0;
  struct matrix *mat = newMat(lines, cols);
  buildMap(mat);

  size_t posX = 0, posY = 0, X = 0, Y = 0;
  struct termios info;
  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);

  
  while(isAlive)
  {
    if(bomb)
    {
      clock_gettime(CLOCK_MONOTONIC, &start);
      if(start.tv_sec >= end.tv_sec)
      {
        kboom(mat, Y, X, 2);
        bomb = 0;
      }
    }


    printMat(mat);
    char c = getchar();

    if(c == 's' || c == '2')
    {
      if(posY + 1 < mat->lines && mat->data[(posY + 1) * mat->cols + posX] == _BGN)
      {
        if(mat->data[posY * mat->cols + posX] == _PLAYER)
          mat->data[posY * mat->cols + posX] = _BGN;
        ++posY;
        mat->data[posY * mat->cols + posX] = _PLAYER;
      }
    }
    else if(c == 'z' || c == '8')
    {
      if(posY != 0 && mat->data[(posY - 1) * mat->cols + posX] == _BGN)
      {
        if(mat->data[posY * mat->cols + posX] == _PLAYER)
          mat->data[posY * mat->cols + posX] = _BGN;
        --posY;
        mat->data[posY * mat->cols + posX] = _PLAYER;
      }
    }
    else if(c == 'd' || c == '6')
    {
      if(posX + 1 < mat->cols && mat->data[posY * mat->cols + posX + 1] == _BGN)
      {
        if(mat->data[posY * mat->cols + posX] == _PLAYER)
          mat->data[posY * mat->cols + posX] = _BGN;
        ++posX;
        mat->data[posY * mat->cols + posX] = _PLAYER;
      }
    }
    else if(c == 'q' || c == '4')
    {
      if(posX != 0 && mat->data[posY * mat->cols + posX - 1] == _BGN)
      {
        if(mat->data[posY * mat->cols + posX] == _PLAYER)
          mat->data[posY * mat->cols + posX] = _BGN;
        --posX;
        mat->data[posY * mat->cols + posX] = _PLAYER;
      }
    }
    else if(c == ' ')
    {
      if(mat->data[posY * mat->cols + posX] != _BOMB && bomb == 0)
      {
        X = posX;
        Y = posY;
        mat->data[posY * mat->cols + posX] = _BOMB;
        bomb = 1;
        clock_gettime(CLOCK_MONOTONIC, &end);
        end.tv_sec += 3;
      }
      if(mat->data[posY * mat->cols + posX] != _BOMB && bomb == 0)
        mat->data[posY * mat->cols + posX] = _BOMB;
    }
    else
      isAlive = 0;
    printf("\e[1;1H\e[2J");
  }
  freeMat(mat);

}

int main()
{
  size_t lines = 13;
//  printf("lines & cols ? : ");
//  scanf("%zu", &lines);
  game(lines, lines * 2);

  return 0;
}
