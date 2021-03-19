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

/*void *q2RobotRoutine(void *arg)
{
  Robot robot = (Robot)arg;
  Task task = robot->task;
#ifdef DEBUG
  debug_printf(__func__, "Robot%c[%d] starts...\n",
               RobotTypeToChar(robot->robotType), robot->id);
#endif
  int jobID, freeSpace;
  sem_wait(&task->space_lock);
  sem_wait(&task->binary_lock);
  freeSpace = getNumFreeSpace();
  if (freeSpace == 1)
  {flushed_printf("1 space only\n");
    if (task->body_jumped_in_queue)
    {
      if (task->car_jumped_in_queue)
        sem_post(&task->binary_lock);
      else
      {
        task->car_jumped_in_queue = true;
        sem_post(&task->binary_lock);
        flushed_printf("car \n");
        simpleWork(CAR, robot);
      }
    }
    else
    {
      task->body_jumped_in_queue = true;
      sem_post(&task->binary_lock);
      flushed_printf("body \n");
      simpleWork(BODY, robot);
    }
  }
  else
  {
    while (!queueIsEmpty(task->jobQ))
    {
      queueDequeueFront(task->jobQ, &jobID);
#ifdef DEBUG
      debug_printf(__func__, "Robot%c[%d]: working on job %d...\n",
                   RobotTypeToChar(robot->robotType), robot->id, jobID);
#endif
      if (jobID == 6 && task->body_jumped_in_queue)
      {
        task->body_jumped_in_queue = false;
        sem_post(&task->binary_lock);
        break;
      }
      
      if (jobID == 7 && task->car_jumped_in_queue)
      {
        task->car_jumped_in_queue = false;
        sem_post(&task->binary_lock);
        break;
      }
      sem_post(&task->binary_lock);
      simpleWork(jobID, robot);
    }
  }
  pthread_exit(NULL);
}
*/