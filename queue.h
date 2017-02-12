# ifndef QUEUE_H_
# define QUEUE_H_

# define _XOPEN_SOURCE 500

# include "vector.h"

# include <unistd.h>

struct list {
  struct list *next;
  time_t time;
  int pl, X, Y;
};
 
struct queue {
  struct list *store;
  size_t       size;
};
 
struct queue *queue_init();
 
void queue_push(struct queue *queue, int pl, int X, int Y, time_t time);

struct list *queue_pop(struct queue *queue);
 
void freeQueue(struct queue *queue);

# endif /* QUEUE_H_ */
