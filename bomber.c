# define _XOPEN_SOURCE 500

# include "vector.h"

# include <termios.h>
# include <unistd.h>

struct vector *generate_Bomb(struct matrix *mat)
{
  struct vector *vect = vector_make(16);
  struct Tuple *tup;

  for(size_t i = 0; i < mat->lines; ++i)
  {
    for(size_t j = 0; j < mat->cols; ++j)
    {
      if(mat->data[i * mat->cols + j] == _BGN && rand() % 15 == 0)
      {
        tup = malloc(sizeof(struct Tuple));
        tup->t1 = i;
        tup->t2 = j;
        vector_push_back(vect, tup);

        mat->data[i * mat->cols + j] = _BOMB;
      }
    }
  }
  return vect;
}

void clear_Bomb(struct matrix *mat)
{
  for(size_t i = 0; i < mat->lines; ++i)
  {
    for(size_t j = 0; j < mat->cols; ++j)
    {
      if(mat->data[i * mat->cols + j] == _KBOOM)
        mat->data[i * mat->cols + j] = _BGN;
    } 
  }
}

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

int kboom(struct matrix *mat, struct player *player, int lines, int cols)
{
  int x = cols;
  int y = lines;
  int left = player->range;
  int right = left;
  int up = left;
  int down = up;
  int isAlive = 1;

  if(player->X == player->posX && player->Y == player->posY)
    isAlive = 0;
 
  mat->data[y * mat->cols + x] = _KBOOM;

  for (int i = x - 1; i >= 0 && left >= 0 && mat->data[y * mat->cols + i] != _WALLU; --i, --left)
  {
    if(mat->data[y * mat->cols + i] == _BGN)
      mat->data[y * mat->cols + i] = _KBOOM;
    else if (mat->data[y * mat->cols + i] == _WALLE)
    {
      left = 0;
      if(rand() % 1 == 0)
        mat->data[y * mat->cols + i] = _EXT;
      else
       mat->data[y * mat->cols + i] = _BGN;
    }
    else if(mat->data[y * mat->cols + i] == _PLAYER || mat->data[y * mat->cols + i] == _PLAYER2)
     isAlive = 0;
  }

  for (int j = y - 1; j >= 0 && up >= 0 && mat->data[j * mat->cols + x] != _WALLU; --j, --up)
  {
    if(mat->data[j * mat->cols + x] == _BGN)
      mat->data[j * mat->cols + x] = _KBOOM;
    else if (mat->data[j * mat->cols + x] == _WALLE)
    {
      up = 0;
      if(rand() % 2 == 0)
        mat->data[j * mat->cols + x] = _EXT;
      else
        mat->data[j * mat->cols + x] = _BGN;
    }
    else if(mat->data[j * mat->cols + x] == _PLAYER || mat->data[j * mat->cols + x] == _PLAYER2)
     isAlive = 0;
  }

  for (int i = x + 1; i < (int)mat->cols && right >= 0 && mat->data[y * mat->cols + i] != _WALLU; ++i, --right)
  {
    if(mat->data[y * mat->cols + i] == _BGN)
      mat->data[y * mat->cols + i] = _KBOOM;
    else if (mat->data[y * mat->cols + i] == _WALLE)
    {
      if(rand() % 2 == 0)
        mat->data[y * mat->cols + i] = _DIE;
      else
        mat->data[y * mat->cols + i] = _BGN;
      right = 0;
    }
    else if(mat->data[y * mat->cols + i] == _PLAYER || mat->data[y * mat->cols + i] == _PLAYER2)
     isAlive = 0;
  }

  for (int j = y + 1; j < (int)mat->lines && down >= 0 && mat->data[j * mat->cols + x] != _WALLU; ++j, --down)
  {
    if(mat->data[j * mat->cols + x] == _BGN)
      mat->data[j * mat->cols + x] = _KBOOM;
    else if (mat->data[j * mat->cols + x] == _WALLE)
    {
      down = 0;
      if(rand() % 2 == 0)
        mat->data[j * mat->cols + x] = _DIE;
      else
       mat->data[j * mat->cols + x] = _BGN;
    }
    else if(mat->data[j * mat->cols + x] == _PLAYER || mat->data[j * mat->cols + x] == _PLAYER2)
      isAlive = 0;
  }

  return isAlive;
}

void game(size_t lines, size_t cols)
{
  int data, generat = 0, expl3 = 0;
  struct timespec current, end, end2, end3;
  struct matrix *mat = newMat(lines, cols);
  struct vector *vect;
  struct player *player = NULL, *player1 = newPlayer(_PLAYER, mat);
  struct player *player2 = newPlayer(_PLAYER2, mat);
  buildMap(mat);
  randomMap(mat);

  struct termios info;
  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);
  
  while(player1->isAlive && player2->isAlive)
  {
    if(player1->bomb || player2->bomb || generat)
    {
      clock_gettime(CLOCK_MONOTONIC, &current);
      if(player1->bomb)
      {
        if(current.tv_sec >= end.tv_sec)
        {
          if(player1->expl)
          {
            end_Bomb(mat, player1->Y, player1->X);
            player1->expl = 0;
            player1->bomb = 0;
          }
          else
          {
            player1->isAlive = kboom(mat, player1, player1->Y, player1->X);
            clock_gettime(CLOCK_MONOTONIC, &end);
            end.tv_sec += 1;
            player1->expl = 1;
          }
        }
      }
      if(player2->bomb)
      {
        if(current.tv_sec >= end2.tv_sec)
        {
          if(player2->expl)
          {
            end_Bomb(mat, player2->Y, player2->X);
            player2->expl = 0;
            player2->bomb = 0;
          }
          else
          {
            player2->isAlive = kboom(mat, player2, player2->Y, player2->X);
            clock_gettime(CLOCK_MONOTONIC, &end2);
            end2.tv_sec += 1;
            player2->expl = 1;
          }
        }
      }
      if(generat)
      {
        if(current.tv_sec >= end3.tv_sec)
        {
          if(!expl3)
          {
            for(int i = 0; i < vect->size; ++i)
              player1->isAlive = kboom(mat, player1, vect->data[i]->t1, vect->data[i]->t2);
            freeVect(vect);
            end3.tv_sec += 1;
            expl3 = 1;
          }
          else
          {
//            for(int i = 0; i < vect->size; ++i)
//              end_Bomb(mat, vect->data[i]->t1, vect->data[i]->t2);
            clear_Bomb(mat);
            expl3 = 0;
            generat = 0;
//            freeVect(vect);
          }
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
        data = mat->data[(player->posY + 1) * mat->cols + player->posX];
        if(data >= _BGN)
        {
          if(mat->data[(player->posY + 1) * mat->cols + player->posX] == _EXT)
            ++player->range;
          else if(data == _DIE)
          {
            vect = generate_Bomb(mat);
            generat = 1;
            clock_gettime(CLOCK_MONOTONIC, &end3);
            end3.tv_sec += 3;
          }

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
        if(mat->data[(player->posY - 1) * mat->cols + player->posX] >= _BGN)
        {
          if(mat->data[(player->posY - 1) * mat->cols + player->posX] == _EXT)
            ++player->range;
          else if(mat->data[(player->posY - 1) * mat->cols + player->posX] == _DIE)
          {
            vect = generate_Bomb(mat);
            generat = 1;
            clock_gettime(CLOCK_MONOTONIC, &end3);
            end3.tv_sec += 3;

          }

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
        if(mat->data[player->posY * mat->cols + player->posX + 1] >= _BGN)
        {
          if(mat->data[player->posY * mat->cols + player->posX + 1] == _EXT)
            ++player->range;
          else if(mat->data[player->posY * mat->cols + player->posX + 1] == _DIE)
          {
            vect = generate_Bomb(mat);
            generat = 1;
            clock_gettime(CLOCK_MONOTONIC, &end3);
            end3.tv_sec += 3;
          }

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
        if(mat->data[player->posY * mat->cols + player->posX - 1] >= _BGN)
        {
          if(mat->data[player->posY * mat->cols + player->posX - 1] == _EXT)
            ++player->range;
          else if(mat->data[player->posY * mat->cols + player->posX - 1] == _DIE)
          {
            generat = 1;
            vect = generate_Bomb(mat);
            clock_gettime(CLOCK_MONOTONIC, &end3);
            end3.tv_sec += 3;
          }

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

      if(mat->data[player->posY * mat->cols + player->posX] != _BOMB && player->bomb == 0)
      {
        player->X = player->posX;
        player->Y = player->posY;
        mat->data[player->posY * mat->cols + player->posX] = _BOMB;
        player->bomb = 1;
        if(c == ' ')
        {
          clock_gettime(CLOCK_MONOTONIC, &end);
          end.tv_sec += 3;
        }
        else
        {
          clock_gettime(CLOCK_MONOTONIC, &end2);
          end2.tv_sec += 3;
        }
      }
    }
    printf("\e[1;1H\e[2J");
  }
  freeMat(mat);
  free(player1);
  free(player2);
}

int main()
{
  size_t lines = 21;
  game(lines, lines * 2);

  return 0;
}
