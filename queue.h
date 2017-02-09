# ifndef QUEUE_H_
# define QUEUE_H_

# define _XOPEN_SOURCE 500

# include "vector.h"

# include <unistd.h>

struct tup
{
  struct Tuple *t1;
  struct timespec t2;
};
 
struct list {
  struct list *next;
  void        *data;
};
 
struct queue {
  struct list *store;
  size_t       size;
};
 
struct queue *queue_init();
 
int queue_is_empty(struct queue *queue);
 
void queue_push(struct queue *queue, void *elm);
 
void* queue_pop(struct queue *queue);
 
# endif /* QUEUE_H_ */
