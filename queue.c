# include "queue.h"

struct queue *queue_init()
{
  struct queue *queue = malloc(sizeof(struct queue));
  queue->store = malloc(sizeof(struct list));
  queue->store->next = NULL;
  queue->size = 0;
  return queue;
}

void queue_push(struct queue *queue, void *elm)
{
  struct list *list = malloc(sizeof(struct list));
  list->data = elm;
  
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

void* queue_pop(struct queue *queue)
{
  if (queue->size == 0)
    return NULL;

  --queue->size;
  struct list *list = queue->store->next;
  void *elm = list->data;

  queue->store->next = queue->store->next->next;

  free(list);

  return elm;
}

void freeQueue(struct queue *queue)
{
  struct list *list = NULL;
  while(queue->size > 0)
  {
    list = queue->store;
    queue->store = queue->store->next;
    free(list->data);
    free(list);
    --queue->size;
  }
}
