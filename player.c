# include "player.h"

struct player *newPlayer(int val, struct matrix *mat)
{
  struct player *player = malloc(sizeof(struct player));
  player->isAlive = 1;
  player->value = val;
  player->range = 0;
  player->maxBomb = 0;
  player->nbBomb = 0;

  if(player->value == _PLAYER)
  {
    player->posX = 0;
    player->posY = 0;
    player->X = 0;
    player->Y = 0;
  }
  else if(player->value == _PLAYER2)
  {
    player->posX = mat->cols - 1;
    player->posY = mat->lines - 1;
    player->X = mat->cols - 1;
    player->Y = mat->lines - 1;
  }
  return player;
}
