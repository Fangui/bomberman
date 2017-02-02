# ifndef MATRIX_H_
# define MATRIX_H_
# define _BOMB 666
# define _PLAYER 42
# define _BGN 41

# include <stdio.h>
# include <stdlib.h>
# include <time.h>

struct matrix
{
  int *data;
  size_t lines;
  size_t cols;
};

struct matrix *newMat(size_t lines, size_t cols);

struct matrix *cloneMat(struct matrix *mat);

void freeMat(struct matrix *mat);

void printMat(struct matrix *mat);

void buildMap(struct matrix *mat);

# endif
