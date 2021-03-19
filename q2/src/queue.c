#include "../include/queue.h"
#include <semaphore.h>

Queue queueCreate(unsigned capacity)
{
  Queue q = malloc(sizeof(queue_t));
  q->capacity = capacity;
  q->front = q->size = 0;
  q->rear = capacity - 1;
  q->array = malloc(q->capacity * sizeof(*q->array));
  sem_init(&q->sema, 0, 1);
  return q;
}

void queueDestroy(Queue q)
{
  sem_destroy(&q->sema);
  free(q->array);
  free(q);
}

int queueIsFull(Queue q)
{
  sem_wait(&q->sema);
  int temp = (q->size == q->capacity);
  sem_post(&q->sema);
  return (temp);
}

int queueIsEmpty(Queue q)
{
  sem_wait(&q->sema);
  int temp = (q->size == 0);
  sem_post(&q->sema);
  return (temp);
}

int queueEnqueue(Queue q, int *value)
{
  if (queueIsFull(q))
    return -1;

  sem_wait(&q->sema);
  q->rear = (q->rear + 1) % q->capacity;
  q->array[q->rear] = *value;
  q->size = q->size + 1;
  sem_post(&q->sema);
  return 0;
}

int queueDequeueFront(Queue q, int *container)
{
  if (queueIsEmpty(q))
    return -1;

  sem_wait(&q->sema);
  *container = q->array[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size = q->size - 1;
  sem_post(&q->sema);
  return 0;
}

int queueDequeueRear(Queue q, int *container)
{
  if (queueIsEmpty(q))
    return -1;
  sem_wait(&q->sema);

  *container = q->array[q->rear];
  q->rear = (q->capacity + q->rear - 1) % q->capacity;
  q->size = q->size - 1;
  sem_post(&q->sema);
  return 0;
}

int *queueFront(Queue q)
{
  if (queueIsEmpty(q))
    return NULL;
  sem_wait(&q->sema);
  int *temp = &q->array[q->front];
  sem_post(&q->sema);
  return temp;
}

int *queueRear(Queue q)
{
  if (queueIsEmpty(q))
    return NULL;
  sem_wait(&q->sema);
  int *temp = &q->array[q->rear];
  sem_post(&q->sema);
  return temp;
}

void queuePrint(Queue q)
{
  sem_wait(&q->sema);
  printf("Queue(^: front, *: rear): [");
  int i;
  for (i = 0; i < q->capacity; i++)
  {
    printf("%d", q->array[i]);
    if (i == q->front)
      printf("^");
    if (i == q->rear)
      printf("*");
    i == q->capacity - 1 ? printf("] ") : printf(", ");
  }
  printf("size=%d, capacity=%d\n", q->size, q->capacity);
  sem_post(&q->sema);
  fflush(stdout);
}
