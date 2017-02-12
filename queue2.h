# ifndef QUEUE2_H_
# define QUEUE2_H_

# include "queue.h"


struct list2 {
  struct list2 *next;
  time_t time;
  size_t X, Y;
};
 
struct queue2 {
  struct list2 *store;
  size_t       size;
};
 
struct queue2 *queue2_init();
 
void queue2_push(struct queue2 *queue, size_t X, size_t Y, time_t time);

struct list2 *queue2_pop(struct queue2 *queue);
 
void freeQueue2(struct queue2 *queue);

# endif /* QUEUE2_H_ */
