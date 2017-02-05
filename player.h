# ifndef PLAYER_H_
# define PLAYER_H_

# include "map.h"

struct player
{
  int isAlive, value;
  size_t posX, posY, nbBomb, X, Y, range;
};

struct player *newPlayer(int val);
# endif