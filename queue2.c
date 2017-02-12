# include "queue2.h"

struct queue2 *queue2_init()
{
  struct queue2 *queue = calloc(1, sizeof(struct queue2));
  queue->store = calloc(1, sizeof(struct list2));
  return queue;
}

void queue2_push(struct queue2 *queue, size_t X, size_t Y, time_t time)
{
  struct list2 *list = malloc(sizeof(struct list2));
  list->X = X;
  list->Y = Y;
  list->time = time;
  
  if(queue->size == 0)
  {
    queue->store = list;
    list->next = list;
  }

  else
  {
    list->next = queue->store->next;
    queue->store->next = list;
    queue->store = list;
  }
  ++queue->size;
}

struct list2 *queue2_pop(struct queue2 *queue)
{
  if (queue->size == 0)
    return NULL;

  --queue->size;
  struct list2 *list = queue->store->next;

  queue->store->next = queue->store->next->next;

  return list;
}

void freeQueue2(struct queue2 *queue)
{
  struct list2 *list = NULL;
  while(queue->size > 0)
  {
    list = queue->store;
    queue->store = queue->store->next;
    free(list);
    --queue->size;
  }
}
