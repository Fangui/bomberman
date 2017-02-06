# define _XOPEN_SOURCE 500

# include "player.h"

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
 
  int bomb = 0, expl = 0;
  struct matrix *mat = newMat(lines, cols);
  struct player *player = NULL, *player1 = newPlayer(_PLAYER, mat);
  struct player *player2 = newPlayer(_PLAYER2, mat);
  buildMap(mat);

  struct termios info;
  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);
  
  while(player1->isAlive && player2->isAlive)
  {
    if(bomb)
    {
      clock_gettime(CLOCK_MONOTONIC, &current);
      if(current.tv_sec >= end.tv_sec)
      {
        if(expl)
        {
          end_Bomb(mat, player1->Y, player1->X);
          expl = 0;
          bomb = 0;
        }
        else
        {
          player1->isAlive = kboom(mat, player1->Y, player1->X, player1->range);
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
      if(c == 's')
        player = player1;
      else
        player = player2;

      if(player->posY + 1 < mat->lines)
      {
        if(mat->data[(player->posY + 1) * mat->cols + player->posX] == _BGN)
        {
          if(mat->data[player->posY * mat->cols + player->posX] == player->value)
            mat->data[player->posY * mat->cols + player->posX] = _BGN;
          ++player->posY;
          mat->data[player->posY * mat->cols + player->posX] = player->value;
        }
        else if(mat->data[(player->posY + 1) * mat->cols + player->posX] == _KBOOM)
         player->isAlive = 0;
      }
    }
    else if(c == 'z' || c == '8')
    {
      if(c == 'z')
        player = player1;
       else
        player = player2;

      if(player->posY != 0)
      {
        if(mat->data[(player->posY - 1) * mat->cols + player->posX] == _BGN)
        {
          printf("%d\n", player->value);
          if(mat->data[player->posY * mat->cols + player->posX] == player->value)
            mat->data[player->posY * mat->cols + player->posX] = _BGN;
          --player->posY;
          mat->data[player->posY * mat->cols + player->posX] = player->value;
        }
        else if(mat->data[(player->posY - 1) * mat->cols + player->posX] == _KBOOM)
          player->isAlive = 0;
      }
    }
    else if(c == 'd' || c == '6')
    {
      if(c == 'd')
        player = player1;
      else
        player = player2;

      if(player->posX + 1 < mat->cols)
      {
        if(mat->data[player->posY * mat->cols + player->posX + 1] == _BGN)
        {
          if(mat->data[player->posY * mat->cols + player->posX] == player->value)
            mat->data[player->posY * mat->cols + player->posX] = _BGN;
          ++player->posX;
          mat->data[player->posY * mat->cols + player->posX] = player->value;
        }
        else if(mat->data[player->posY * mat->cols + player->posX + 1] == _KBOOM)
         player->isAlive = 0;
      }
    }
    else if(c == 'q' || c == '4')
    {
      if(c == 'q')
        player = player1;
      else
        player = player2;

      if(player->posX != 0)
      {
        if(mat->data[player->posY * mat->cols + player->posX - 1] == _BGN)
        {
          if(mat->data[player->posY * mat->cols + player->posX] == player->value)
            mat->data[player->posY * mat->cols + player->posX] = _BGN;
          --player->posX;
          mat->data[player->posY * mat->cols + player->posX] = player->value;
        }
        else if(mat->data[player->posY * mat->cols + player->posX - 1] == _KBOOM)
          player->isAlive = 0;
      }
    }
    else if(c == ' ' || c == '5')
    {
      if(c == ' ')
        player = player1;
      else
        player = player2;

      if(mat->data[player->posY * mat->cols + player->posX] != _BOMB && bomb == 0)
      {
        player->X = player->posX;
        player->Y = player->posY;
        mat->data[player->posY * mat->cols + player->posX] = _BOMB;
        bomb = 1;
        clock_gettime(CLOCK_MONOTONIC, &end);
        end.tv_sec += 2;
      }
    }
    printf("\e[1;1H\e[2J");
  }
  freeMat(mat);
  free(player);
  free(player2);
}

int main()
{
  size_t lines = 13;
//  printf("lines & cols ? : ");
//  scanf("%zu", &lines);
  game(lines, lines * 2);

  return 0;
}
