# include "queue.h"

struct queue *queue_init()
{
  struct queue *queue = calloc(1, sizeof(struct queue));
  return queue;
}

void queue_push(struct queue *queue, int player, int X, int Y, time_t time)
{
  struct list *list = malloc(sizeof(struct list));
  struct data *data = malloc(sizeof(struct data));
  list->data = data;
  list->data->pl = player;
  list->data->X = X;
  list->data->Y = Y;
  list->data->time = time;
  
  if(queue->size == 0)
  {
    queue->store = list;
    list->next = list;
  }

  else
  {
    list->next = queue->store->next;
    queue->store->next = list;
//    queue->store = list;
  }
  ++queue->size;
}

struct data *queue_pop(struct queue *queue)
{
  if (queue->size == 0)
    return NULL;

  --queue->size;

  struct list *list = queue->store;
  struct list *last = list->next;
  while(last->next != list)
    last = last->next;

  queue->store = last;
  last->next = list->next;
  queue->store = last;

  struct data *data = list->data;

  free(list);

  return data;
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
  free(queue);
}
