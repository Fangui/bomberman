# include "player.h"

struct player *newPlayer(int val)
{
  struct player *player = malloc(sizeof(struct player));
  player->isAlive = 1;
  player->value = val;
  player->range = 1;

  if(player->value == _PLAYER)
  {
    player->posX = 0;
    player->posY = 0;
    player->X = 0;
    player->Y = 0;
  }
}
