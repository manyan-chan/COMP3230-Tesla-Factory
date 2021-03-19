#include "../include/production.h"
#include "../include/utils.h"
#include "../include/scheduler.h"
#include "../include/robot.h"
#include "../include/queue.h"

static int num_cars;
static int num_spaces;
static int num_typeA;
static int num_typeB;
static int num_typeC;
static int total_num_robots;

void prioirity(ReadyQueue_t *First, ReadyQueue_t *Second, ReadyQueue_t *Third)
{
  int dummy;
  while (true)
  {
    if (!queueIsEmpty(First->readyQ))
    {
      queueDequeueFront(First->readyQ, &dummy);
      sem_post(&First->Lock);
      break;
    }
    else if (!queueIsEmpty(Second->readyQ))
    {
      queueDequeueFront(Second->readyQ, &dummy);
      sem_post(&Second->Lock);
      break;
    }
    else if (!queueIsEmpty(Third->readyQ))
    {
      queueDequeueFront(Third->readyQ, &dummy);
      sem_post(&Third->Lock);
      break;
    }
  }
}
void schedule(Task T, int jobID)
{
#ifdef DEBUG
  debug_printf(__func__, "Scheduling job %d...\n", jobID);
#endif
  switch (jobID)
  {
  case 0:
    prioirity(&T->C, &T->B, &T->A);
    break;

  case 1:
    prioirity(&T->C, &T->A, &T->B);
    break;

  case 2:
    prioirity(&T->A, &T->B, &T->C);
    break;

  case 3:
    prioirity(&T->A, &T->B, &T->C);
    break;

  case 4:
    prioirity(&T->C, &T->B, &T->A);
    break;

  case 5:
    prioirity(&T->A, &T->C, &T->B);
    break;

  case 6:
    prioirity(&T->B, &T->A, &T->C);
    break;

  case 7:
    prioirity(&T->C, &T->B, &T->A);
    break;
  }
#ifdef DEBUG
  debug_printf(__func__, "Done\n");
#endif
}

void startProduction()
{
  /* Init robots */
  Robot *robotsA = calloc(num_typeA, sizeof(Robot_t));
  Robot *robotsB = calloc(num_typeB, sizeof(Robot_t));
  Robot *robotsC = calloc(num_typeC, sizeof(Robot_t));
  /* Init robots end*/

  /* Prepare task */
  Task task = calloc(1, sizeof(Task_t));
  task->jobQ = queueCreate(num_cars * 17);
  task->A.readyQ = queueCreate(num_typeA);
  task->B.readyQ = queueCreate(num_typeB);
  task->C.readyQ = queueCreate(num_typeC);
  sem_init(&task->A.Lock, 0, 1);
  sem_init(&task->B.Lock, 0, 1);
  sem_init(&task->C.Lock, 0, 1);
  sem_init(&task->scheduler_wait, 0, 1);

  sem_wait(&task->A.Lock);
  sem_wait(&task->B.Lock);
  sem_wait(&task->C.Lock);
  sem_wait(&task->scheduler_wait);

  //optimal solution
  int arr2[17] = {0, 6, 1, 2, 7, 3, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4};
  int arr3[17] = {0, 1, 6, 2, 3, 7, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4};
  int arr4[17] = {0, 1, 2, 6, 3, 5, 7, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4};
  int arr5[17] = {0, 1, 2, 6, 3, 5, 5, 7, 5, 5, 4, 4, 4, 4, 4, 4, 4};
  int arr6[17] = {0, 1, 2, 6, 3, 5, 5, 5, 7, 5, 4, 4, 4, 4, 4, 4, 4};
  int arr7[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 7, 4, 4, 4, 4, 4, 4, 4};
  int arr8[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 7, 4, 4, 4, 4, 4, 4};
  int arr9[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 7, 4, 4, 4, 4, 4};
  int arr10[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 4, 7, 4, 4, 4, 4};
  int arr11[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 4, 4, 7, 4, 4, 4};
  int arr12[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 4, 4, 4, 7, 4, 4};
  int arr13[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 7, 4};
  int arr14[17] = {0, 1, 2, 6, 3, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 7};
  int space = num_spaces;

  int *arr[17];

  switch (space)
  {
  case 2:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr2[i];
    break;
  case 3:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr3[i];
    break;
  case 4:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr4[i];
    break;
  case 5:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr5[i];
    break;
  case 6:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr6[i];
    break;
  case 7:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr7[i];
    break;
  case 8:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr8[i];
    break;
  case 9:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr9[i];
    break;
  case 10:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr10[i];
    break;
  case 11:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr11[i];
    break;
  case 12:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr12[i];
    break;
  case 13:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr13[i];
    break;
  default:
    for (int i = 0; i < 17; i++)
      arr[i] = &arr14[i];
    break;
  }

  for (int k = 0; k < num_cars; k++)
  {
    for (int i = 0; i < 17; i++)
    {
      queueEnqueue(task->jobQ, arr[i]);
    }
  }

/* Prepare task end*/

/* Production start */
// Create robot, assign task, and start to work
#ifdef DEBUG
  debug_printf(__func__, "making RobotA...\n");
#endif

  for (int i = 0; i < num_typeA; ++i)
  {
    robotsA[i] = createRobot(TypeA);
    robotsA[i]->task = task;
    pthread_create(&robotsA[i]->pthread, NULL, complexRobotRoutine, robotsA[i]);
  }

//create typeB and typeC robots
#ifdef DEBUG
  debug_printf(__func__, "making RobotB...\n");
#endif
  for (int i = 0; i < num_typeB; ++i)
  {
    robotsB[i] = createRobot(TypeB);
    robotsB[i]->task = task;
    pthread_create(&robotsB[i]->pthread, NULL, complexRobotRoutine, robotsB[i]);
  }

#ifdef DEBUG
  debug_printf(__func__, "making RobotC...\n");
#endif
  for (int i = 0; i < num_typeC; ++i)
  {
    robotsC[i] = createRobot(TypeC);
    robotsC[i]->task = task;
    pthread_create(&robotsC[i]->pthread, NULL, complexRobotRoutine, robotsC[i]);
  }


#ifdef DEBUG
  debug_printf(__func__, "scheduling start...\n");
#endif

//process scheduling
  while (!queueIsEmpty(task->jobQ))
  {
    int jobID;
    jobID = *queueFront(task->jobQ);
    schedule(task, jobID);
    sem_wait(&task->scheduler_wait);
  }

  for (int i = 0; i < num_typeA; i++)
    sem_post(&task->A.Lock);
  for (int i = 0; i < num_typeB; i++)
    sem_post(&task->B.Lock);
  for (int i = 0; i < num_typeC; i++)
    sem_post(&task->C.Lock);

#ifdef DEBUG
  debug_printf(__func__, "scheduling done...\n");
#endif

#ifdef DEBUG
  debug_printf(__func__, "joining pthread...\n");
#endif

  // wait until work done
  for (int i = 0; i < num_typeA; ++i)
  {
    pthread_join(robotsA[i]->pthread, NULL);
  }
  // join typeB and typeC robot threads
  for (int i = 0; i < num_typeB; ++i)
  {
    pthread_join(robotsB[i]->pthread, NULL);
  }
  for (int i = 0; i < num_typeC; ++i)
  {
    pthread_join(robotsC[i]->pthread, NULL);
  }
  /* Production end */

#ifdef DEBUG
  debug_printf(__func__, "joining pthread done\n");
#endif

  /* Release memory */
  for (int i = 0; i < num_typeA; ++i)
    releaseRobot(robotsA[i]);
  for (int i = 0; i < num_typeB; ++i)
    releaseRobot(robotsB[i]);
  for (int i = 0; i < num_typeC; ++i)
    releaseRobot(robotsC[i]);

  sem_destroy(&task->A.Lock);
  sem_destroy(&task->B.Lock);
  sem_destroy(&task->C.Lock);
  sem_destroy(&task->scheduler_wait);
  queueDestroy(task->jobQ);
  free(task);
  free(robotsA);
  free(robotsB);
  free(robotsC);
#ifdef DEBUG
  debug_printf(__func__, "memory released...\n");
#endif
}

int main(int argc, char *argv[])
{
  if (argc < 6)
  {
    printf("Usage: %s <number of cars> <number of spaces> <number of typeA robot> <number of typeB robot> <number of typeC robot>\n", argv[0]);
    return EXIT_SUCCESS;
  }
  // Change to your name and university ID
  printf("Name: CHAN MAN YAN\t UID: 3035582954\n");
  /* Initialization */
  num_cars = atoi(argv[1]);
  num_spaces = atoi(argv[2]);
  num_typeA = atoi(argv[3]);
  num_typeB = atoi(argv[4]);
  num_typeC = atoi(argv[5]);
  total_num_robots = num_typeA + num_typeB + num_typeC;
  printf("Production goal: %d, num space: %d, num typeA: %d, num typeB: %d, num typeC: %d\n",
         num_cars, num_spaces, num_typeA, num_typeB, num_typeC);
  initProduction(num_cars, num_spaces);
  initNumRobot(num_typeA, num_typeB, num_typeC);
  /* Initialization end */

  double timer = getCurrentTime();
  startProduction();
  timer = getCurrentTime() - timer;

  printf("====Final Report====\n");
  printProductionProgress();
  printf("Production time: %lf\n", timer);
  return 0;
}
