# include "queue2.h"

# include <termios.h>

void generate_Bomb(struct matrix *mat, struct vector *vect)
{
  struct Tuple *tup = NULL;
  size_t pos;
  for(size_t i = 0; i < mat->lines; ++i)
    for(size_t j = 0; j < mat->cols; ++j)
    {
      pos = i * mat->cols + j;
      if(mat->data[pos] == _BGN && rand() % 15 == 0)
      {
        tup = malloc(sizeof(struct Tuple));
        tup->t1 = i;
        tup->t2 = j;
        vector_push_back(vect, tup);

        mat->data[pos] = _BOMB;
      }
    }
}

void clear_Bomb(struct matrix *mat)
{
  size_t pos;
  for(size_t i = 0; i < mat->lines; ++i)
    for(size_t j = 0; j < mat->cols; ++j)
    {
      pos = i * mat->cols + j;
      if(mat->data[pos] == _KBOOM)
        mat->data[pos] = _BGN;
    } 
}

void end_Bomb(struct matrix *mat, int lines, int cols, int field)
{
  int range = field, pos;

  mat->data[lines * mat->cols + cols] = _BGN;

  for(int i = lines - 1; i >= 0 && range >= 0; --i, --range)
  {
    pos = i * mat->cols + cols;
    if(mat->data[pos] == _KBOOM) 
      mat->data[pos] = _BGN;
  }

  range = field;
  for(int j = cols - 1; j >= 0 && range >= 0; --j, --range)
  {
    pos = lines * mat->cols + j;
    if(mat->data[pos] == _KBOOM)
      mat->data[pos] = _BGN;
  }

  range = field;
  for(int i = lines + 1; i < (int) mat->lines && range >= 0; ++i, --range)
  {
    pos = i * mat->cols + cols;
    if(mat->data[pos] == _KBOOM)
      mat->data[pos] = _BGN;
  }

  range = field;
  for(int j = cols + 1; j < (int) mat->cols && range >= 0; ++j, -- field, --range)
  {
    pos = lines * mat->cols + j;
    if(mat->data[pos] == _KBOOM)
      mat->data[pos] = _BGN;
  }
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
  int r, pos;

  if(cols == (int)player->posX && lines == (int)player->posY)
    isAlive = 0;
 
  mat->data[y * mat->cols + x] = _KBOOM;

  for (int i = x - 1; i >= 0 && left >= 0 && mat->data[y * mat->cols + i] != _WALLU; --i, --left)
  {
    pos = y * mat->cols + i;
    if(mat->data[pos] == _BGN)
      mat->data[pos] = _KBOOM;
    else if (mat->data[pos] == _WALLE)
    {
      left = 0;
      r = rand() % 50;
      if(r > 25)
        mat->data[pos] = _BGN;
      else if(r > 15)
        mat->data[pos] = _EXT;
      else if(r > 8)
        mat->data[pos] = _ADD;
      else if(r == 0)
        mat->data[pos] = _DIE;
    }
    else if(mat->data[pos] == _PLAYER || mat->data[pos] == _PLAYER2)
     isAlive = 0;
  }

  for (int j = y - 1; j >= 0 && up >= 0 && mat->data[j * mat->cols + x] != _WALLU; --j, --up)
  {
    pos = j * mat->cols + x;
    if(mat->data[pos] == _BGN)
      mat->data[pos] = _KBOOM;
    else if (mat->data[pos] == _WALLE)
    {
      up = 0;
      r = rand() % 50;
      if(r > 25)
        mat->data[pos] = _BGN;
      else if(r > 15)
        mat->data[pos] = _EXT;
      else if(r > 8)
        mat->data[pos] = _ADD;
      else if(r == 0)
        mat->data[pos] = _DIE;
    }
    else if(mat->data[pos] == _PLAYER || mat->data[pos] == _PLAYER2)
     isAlive = 0;
  }

  for (int i = x + 1; i < (int)mat->cols && right >= 0 && mat->data[y * mat->cols + i] != _WALLU; ++i, --right)
  {
    pos = y * mat->cols + i;
    if(mat->data[pos] == _BGN)
      mat->data[pos] = _KBOOM;
    else if (mat->data[pos] == _WALLE)
    {
      right = 0;
      r = rand() % 50;
      if(r > 25) 
        mat->data[pos] = _BGN;
      else if(r > 15)
        mat->data[pos] = _EXT;
      else if(r > 8)
        mat->data[pos] = _ADD;
      else if(r == 0)
        mat->data[pos] = _DIE;
    }
    else if(mat->data[pos] == _PLAYER || mat->data[pos] == _PLAYER2)
     isAlive = 0;
  }

  for (int j = y + 1; j < (int)mat->lines && down >= 0 && mat->data[j * mat->cols + x] != _WALLU; ++j, --down)
  {
    pos = j * mat->cols + x;
    if(mat->data[pos] == _BGN)
      mat->data[pos] = _KBOOM;
    else if (mat->data[pos] == _WALLE)
    {
      down = 0;
      r = rand() % 50;
      if(r > 25) 
        mat->data[pos] = _BGN;
      else if(r > 15)
        mat->data[pos] = _EXT;
      else if(r > 8)
        mat->data[pos] = _ADD;
      else if(r == 0)
        mat->data[pos] = _DIE;
    }
    else if(mat->data[pos] == _PLAYER || mat->data[pos] == _PLAYER2)
      isAlive = 0;
  }

  return isAlive;
}

void game(size_t lines, size_t cols)
{
  char c;
  int prev, pos, valid, X, Y, generat = 0, expl3 = 0;
  struct timespec current, end, end3;
  struct matrix *mat = newMat(lines, cols);
  struct vector *vect = vector_make(16);
  struct queue *queue = queue_init();
  struct queue2 *queue2 = queue2_init();
  struct data *data = NULL;
  struct list2 *list2 = NULL;
  struct player *player = NULL, *player1 = newPlayer(_PLAYER, mat);
  struct player *player2 = newPlayer(_PLAYER2, mat);
  struct termios info;

  buildMap(mat);
  randomMap(mat);

  tcgetattr(0, &info);
  info.c_lflag &= ~ICANON;
  info.c_cc[VMIN] = 1;
  info.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &info);
  
  while(player1->isAlive && player2->isAlive)
  {
    if(queue->size > 0 || queue2->size > 0)
    {
      clock_gettime(CLOCK_MONOTONIC, &current);

      while(queue->size > 0 && current.tv_sec >= queue->store->data->time)
      {
        data = queue_pop(queue);
        if(data->pl == 1)
          player = player1;
        else
          player = player2;

        player->isAlive = kboom(mat, player, data->Y, data->X);
        queue2_push(queue2, data->X, data->Y, data->time + 1, player->range);
        --player->nbBomb;
        free(data);
      }

      while(queue2->size > 0 && current.tv_sec >= queue2->store->time)
      {
        list2 = queue2_pop(queue2);
        end_Bomb(mat, list2->Y, list2->X, list2->field);

        free(list2);
      }
    }

    if(generat)
    {
      clock_gettime(CLOCK_MONOTONIC, &current);
      if(current.tv_sec >= end3.tv_sec)
      {
        if(!expl3)
        {
          for(int i = 0; i < vect->size; ++i)
            player1->isAlive = kboom(mat, player1, vect->data[i]->t1, vect->data[i]->t2);
          clearVect(vect);
          end3.tv_sec += 1;
          expl3 = 1;
        }
        else
        {
          clear_Bomb(mat);
          expl3 = 0;
          generat = 0;
        }
      }
    }
    
      
    printMat(mat);
    c = getchar();
    valid = 0;

    if(c == 's' || c == '2' || c == 'z' || c == '8' || c == 'q' || c == 'd' || c == '4' || c == '6')
    {
      if(c == 's' || c == 'z' || c == 'q' || c == 'd')
        player = player1;
      else
        player = player2;

      prev = player->posY * mat->cols + player->posX;

      if( (c == 's' || c == '2') && player->posY + 1 < mat->lines)
      {
        pos = (player->posY + 1) * mat->cols + player->posX;
        if(mat->data[pos] >= _BGN)
        {
          ++player->posY;
          valid = 1;
        }
      }
      else if( (c == 'z' || c == '8') && player->posY != 0)
      {
        pos = (player->posY - 1) * mat->cols + player->posX; 
        if(mat->data[pos] >= _BGN)
        {
          --player->posY;
          valid = 1;
        }
      }
      else if( (c == 'd' || c == '6') && player->posX + 1 < mat->cols)
      {
        pos = player->posY * mat->cols + player->posX + 1;
        if(mat->data[pos] >= _BGN)
        {
          ++player->posX;
          valid = 1;
        }
      }
      else if( (c == 'q' || c == '4') && player->posX != 0)
      {
        pos = player->posY * mat->cols + player->posX - 1;
        if(mat->data[pos] >= _BGN)
        {
          --player->posX;
          valid = 1;
        }
      }
      if(valid && mat->data[pos] == _KBOOM)
      {
        player->isAlive = 0;
        valid = 0;
      }
 
      if(valid)
      {
        pos = player->posY * mat->cols + player->posX;
        if(mat->data[pos] == _EXT)
          ++player->range;
        else if(mat->data[pos] == _DIE)
        {
          generate_Bomb(mat, vect);
          generat = 1;
          clock_gettime(CLOCK_MONOTONIC, &end3);
          end3.tv_sec += 3;
        }
        else if(mat->data[pos] == _ADD)
          ++player->maxBomb;

        if(mat->data[prev] == player->value)
          mat->data[prev] = _BGN;
        mat->data[pos] = player->value;
      }
    }
    else if(c == ' ' || c == '5')
    {
      if(c == ' ')
      {
        valid = 1;
        player = player1;
      }
      else
      {
        valid = 0;
        player = player2;
      }
      pos = player->posY * mat->cols + player->posX;
      if(mat->data[pos] != _BOMB && player->nbBomb <= player->maxBomb)
      {
        X = player->posX;
        Y = player->posY;
        mat->data[pos] = _BOMB;
        ++player->nbBomb;
        clock_gettime(CLOCK_MONOTONIC, &end);
        end.tv_sec += 3;
        queue_push(queue, valid, X, Y, end.tv_sec);
      }
    }
    printf("\e[1;1H\e[2J");
  }
  freeQueue(queue);
  freeQueue2(queue2);
  freeVect(vect);
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
