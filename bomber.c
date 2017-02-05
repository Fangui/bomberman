# define _XOPEN_SOURCE 500

# include "map.h"

# include <termios.h>
# include <unistd.h>

void end_Bomb(struct matrix *mat, int lines, int cols)
{
  mat->data[lines * mat->cols + cols] = _BGN;

  for(int i = lines - 1; i >= 0 && mat->data[i * mat->cols + cols] == _KBOOM; --i)
    mat->data[i * mat->cols + cols] = _BGN;
  for(int j = cols - 1; j >= 0 && mat->data[lines * mat->cols + j] == _KBOOM; --j)
    mat->data[lines * mat->cols + j] = _BGN;
  for(int i = lines + 1; i < (int) mat->lines && mat->data[i * mat->cols + cols] == _KBOOM; ++i)
    mat->data[i * mat->cols + cols] = _BGN;
  for(int j = cols + 1; j < (int) mat->cols && mat->data[lines * mat->cols + j] == _KBOOM; ++j)
    mat->data[lines * mat->cols + j] = _BGN;
}

int kboom(struct matrix *mat, size_t lines, size_t cols, int field)
{
  int x = (int)cols;
  int y = (int)lines;
  int left = field;
  int right = field;
  int up = field;
  int down = field;
  int isAlive = 1;

  mat->data[lines * mat->cols + cols] = _KBOOM;

  for (int i = x - 1; i >= 0 && left >= 0 && mat->data[lines * mat->cols + i] != _WALLU; --i, --left)
  {
    if (mat->data[lines * mat->cols + i] == _WALLE)
      left = 0;
    else if(mat->data[lines * mat->cols + i] == _PLAYER)
     isAlive = 0;

    mat->data[lines * mat->cols + i] = _KBOOM;
  }

  for (int j = y - 1; j >= 0 && up >= 0 && mat->data[j * mat->cols + cols] != _WALLU; --j, --up)
  {
    if (mat->data[j * mat->cols + cols] == _WALLE)
      up = 0;
    else if(mat->data[j * mat->cols + cols] == _PLAYER)
     isAlive = 0;
     
    mat->data[j * mat->cols + cols] = _KBOOM;
  }

  for (int i = x + 1; i < (int)mat->cols && right >= 0 && mat->data[lines * mat->cols + i] != _WALLU; ++i, --right)
  {
    if (mat->data[lines * mat->cols + i] == _WALLE)
      right = 0;
    else if(mat->data[lines * mat->cols + i] == _PLAYER)
     isAlive = 0;
 
    mat->data[lines * mat->cols + i] = _KBOOM;
  }

  for (int j = y + 1; j < (int)mat->lines && down >= 0 && mat->data[j * mat->cols + cols] != _WALLU; ++j, --down)
  {
    if (mat->data[j * mat->cols + cols] == _WALLE)
      down = 0;
    else if(mat->data[j * mat->cols + cols] == _PLAYER)
      isAlive = 0;

    mat->data[j * mat->cols + cols] = _KBOOM;
  }

  return isAlive;
}

void game(size_t lines, size_t cols)
{
  struct timespec current, end;
    
  int isAlive = 1, bomb = 0, expl = 0;
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
      clock_gettime(CLOCK_MONOTONIC, &current);
      if(current.tv_sec >= end.tv_sec)
      {
        if(expl)
        {
          end_Bomb(mat, Y, X);
          expl = 0;
          bomb = 0;
        }
        else
        {
          isAlive = kboom(mat, Y, X, 2);
          clock_gettime(CLOCK_MONOTONIC, &end);
          end.tv_sec += 1;
          expl = 1;
        }
      }
    }
    printMat(mat);
    char c = getchar();

    if(c == 's' || c == '2')
    {
      if(posY + 1 < mat->lines)
      {
        if(mat->data[(posY + 1) * mat->cols + posX] == _BGN)
        {
          if(mat->data[posY * mat->cols + posX] == _PLAYER)
            mat->data[posY * mat->cols + posX] = _BGN;
          ++posY;
          mat->data[posY * mat->cols + posX] = _PLAYER;
        }
        else if(mat->data[(posY + 1) * mat->cols + posX] == _KBOOM)
         isAlive = 0;
      }
    }
    else if(c == 'z' || c == '8')
    {
      if(posY != 0)
      {
        if(mat->data[(posY - 1) * mat->cols + posX] == _BGN)
        {
          if(mat->data[posY * mat->cols + posX] == _PLAYER)
            mat->data[posY * mat->cols + posX] = _BGN;
          --posY;
          mat->data[posY * mat->cols + posX] = _PLAYER;
        }
        else if(mat->data[(posY - 1) * mat->cols + posX] == _KBOOM)
          isAlive = 0;
      }
    }
    else if(c == 'd' || c == '6')
    {
      if(posX + 1 < mat->cols)
      {
        if(mat->data[posY * mat->cols + posX + 1] == _BGN)
        {
          if(mat->data[posY * mat->cols + posX] == _PLAYER)
            mat->data[posY * mat->cols + posX] = _BGN;
          ++posX;
          mat->data[posY * mat->cols + posX] = _PLAYER;
        }
        else if(mat->data[posY * mat->cols + posX + 1] == _KBOOM)
         isAlive = 0;
      }
    }
    else if(c == 'q' || c == '4')
    {
      if(posX != 0)
      {
        if(mat->data[posY * mat->cols + posX - 1] == _BGN)
        {
          if(mat->data[posY * mat->cols + posX] == _PLAYER)
            mat->data[posY * mat->cols + posX] = _BGN;
          --posX;
          mat->data[posY * mat->cols + posX] = _PLAYER;
        }
        else if(mat->data[posY * mat->cols + posX - 1] == _KBOOM)
          isAlive = 0;
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
        end.tv_sec += 2;
      }
    }
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
