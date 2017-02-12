# ifndef PLAYER_H_
# define PLAYER_H_

# include "map.h"

struct player
{
  int isAlive, value, nbBomb, maxBomb;
  size_t posX, posY, range;
};

struct player *newPlayer(int val, struct matrix *mat);

# endif
