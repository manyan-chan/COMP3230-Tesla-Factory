#include "../include/scheduler.h"

void *simpleRobotRoutine(void *arg)
{
  Robot robot = (Robot)arg;
  Task task = robot->task;
#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] starts...\n",
               RobotTypeToChar(robot->robotType), robot->id);
#endif
  int jobID;
  while (!queueIsEmpty(task->jobQ))
  {
    queueDequeueFront(task->jobQ, &jobID);
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d]: working on job %d...\n",
                 RobotTypeToChar(robot->robotType), robot->id, jobID);
#endif

    simpleWork(jobID, robot);
  }
  pthread_exit(NULL);
}

void simpleWork(int jobID, Robot robot)
{
  double timer = getCurrentTime();
  switch (jobID)
  {
  case SKELETON:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making skeleton...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeSkeleton(robot);
    break;
  case ENGINE:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making engine...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeEngine(robot);
    break;
  case CHASSIS:
#ifdef DEBUG
    debug_printf(__func__, "Robot%c[%d] making chassis...\n",
                 RobotTypeToChar(robot->robotType), robot->id);
    fflush(stdout);
#endif
    makeChassis(robot);
    break;
  case BODY:
    makeBody(robot);
    break;
  case WINDOW:
    makeWindow(robot);
    break;
  case TIRE:
    makeTire(robot);
    break;
  case BATTERY:
    makeBattery(robot);
    break;
  case CAR:
    makeCar(robot);
    break;
  default:
    err_printf(__func__, __LINE__, "Error!! Robot%c[%d] gets invalid jobID %d\n",
               RobotTypeToChar(robot->robotType), robot->id, jobID);
    break;
  }
  timer = getCurrentTime() - timer;
#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] job %d done! Time: %lf\n",
               RobotTypeToChar(robot->robotType), robot->id, jobID, timer);
#endif
}

void *complexRobotRoutine(void *arg)
{
  Robot robot = (Robot)arg;
  Task task = robot->task;

#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] starts...\n",
               RobotTypeToChar(robot->robotType), robot->id);
#endif

  while (!queueIsEmpty(task->jobQ))
  {
    int thread_id = (int)robot->pthread;
    switch (RobotTypeToChar(robot->robotType))
    {
    case 'A':
      queueEnqueue(task->A.readyQ, &thread_id);
      sem_wait(&task->A.Lock);
      break;
    case 'B':
      queueEnqueue(task->B.readyQ, &thread_id);
      sem_wait(&task->B.Lock);
      break;
    case 'C':
      queueEnqueue(task->C.readyQ, &thread_id);
      sem_wait(&task->C.Lock);
      break;
    }
    

    int jobID;
    if(queueIsEmpty(task->jobQ))
      break;
    else{
      queueDequeueFront(task->jobQ, &jobID);
      sem_post(&task->scheduler_wait);

#ifdef DEBUG
      debug_printf(__func__, "Robot%c[%d]: working on job %d...\n",
                   RobotTypeToChar(robot->robotType), robot->id, jobID);
#endif
      simpleWork(jobID, robot);
    }
  }
  pthread_exit(NULL);
}