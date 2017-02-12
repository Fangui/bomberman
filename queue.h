# ifndef QUEUE_H_
# define QUEUE_H_

# define _XOPEN_SOURCE 500

# include "vector.h"

# include <unistd.h>

struct data{
  time_t time;
  int pl, X, Y;
};

struct list {
  struct list *next;
  struct data *data;
};
 
struct queue {
  struct list *store;
  size_t       size;
};
 
struct queue *queue_init();
 
void queue_push(struct queue *queue, int pl, int X, int Y, time_t time);

struct data *queue_pop(struct queue *queue);
 
void freeQueue(struct queue *queue);

# endif /* QUEUE_H_ */
