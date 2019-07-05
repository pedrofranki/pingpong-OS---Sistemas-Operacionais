//Pedro Henrique Belotto Frankiewicz
//RA 1189212

#include "datatypes.h"
#include "queue.h"
#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACKSIZE 32768		/* tamanho de pilha das threads */
#define _XOPEN_SOURCE 600	/* para compilar no MacOS */
#define MINPRIO -20
#define MAXPRIO 20
#define TASKAGING -1
#define TICKS 20
#define TIMEMICRO 1000


void dispatcher_body ();
task_t *scheduler();
void tick_count();

long int id = 0, userTasks=0;
task_t mainTask, *execTask, *ant, *taskQueue;
task_t dispatcher;
int ticks = 0;
int idDispacher;
struct sigaction action;
struct itimerval timer;

void pingpong_init (){
    setvbuf (stdout, 0, _IONBF, 0) ;


    mainTask.tid = id++;

    execTask = &mainTask;
    printf("%d\n", execTask->tid);
    idDispacher = task_create(&dispatcher, dispatcher_body, NULL);

    action.sa_handler = tick_count ;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;
    if (sigaction (SIGALRM, &action, 0) < 0)
    {
      perror ("Erro em sigaction: ") ;
      exit (1) ;
    }

    timer.it_value.tv_usec = TIMEMICRO;      // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec  = 0 ;      // primeiro disparo, em segundos
    timer.it_interval.tv_usec = TIMEMICRO ;   // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec  = 0 ;

    if (setitimer (ITIMER_REAL, &timer, 0) < 0)
    {
      perror ("Erro em setitimer: ") ;
      exit (1) ;
    }

    task_yield();
}


int task_create (task_t *task,	void (*start_func)(void *),	 void *arg){
    char* stack;
    //task->tid = id++;

    getcontext(&(task->context));

    stack = malloc(STACKSIZE);
    if (stack)
   {
      task->context.uc_stack.ss_sp = stack ;
      task->context.uc_stack.ss_size = STACKSIZE;
      task->context.uc_stack.ss_flags = 0;
      task->context.uc_link = 0;
   }
   else
   {
      perror ("Erro na criação da pilha: ");
      exit (1);
   }
    makecontext(&(task->context), (void *)(*start_func), 1, arg);

    task->tid = id;
    id++;

    if(task->tid >1){
      queue_append((queue_t**)&taskQueue, (queue_t*)task);
      userTasks++;
      task->queue = &taskQueue;
    }

    //task->tid = id++;
    task->creationTime = systime();
    task->activations=0;
#ifdef DEBUG
    printf("task_create: task %d criada.\n", task->tid);
#endif

    return task->tid;
}


void task_exit (int exitCode){
    execTask->exitTime = systime();
    printf("Task %d exit: execution time %d ms, processor time %d ms, %d activations", execTask->tid,
    execTask->exitTime - execTask->creationTime, execTask->processorTime, execTask->activations);
    #ifdef DEBUG
        printf("task_exit: encerrando task %d.\n", taskExec->tid);
    #endif

    if(execTask == &dispatcher)
      task_switch(&mainTask);
    else
      task_switch(&dispatcher);
}


int task_switch (task_t *task){
    ant = execTask;
    execTask = task;
    #ifdef DEBUG
      printf("task_switch: trocando contexto %d -> %d\n", ant->tid, taskExec->tid);
    #endif
    task->activations++;
    
    if(swapcontext(&(ant->context), &(task->context))<0){
        
        execTask = ant;
        return -1;
    }
    return 0;
}


int task_id (){
    return execTask->tid;
}

void task_suspend (task_t *task, task_t **queue) {
  if(queue != NULL){
    if(task == NULL){
      queue_remove((queue_t**)execTask->queue, (queue_t*)execTask);
      queue_append((queue_t**)queue, (queue_t*)execTask);
    }else{
      queue_remove((queue_t**)task->queue, (queue_t*)task);
      queue_append((queue_t**)queue, (queue_t*)task);
      userTasks--;
      task->queue = queue;
    }
  }
}

void task_resume (task_t *task) {
  queue_remove((queue_t**)task->queue, (queue_t*)task);
  queue_append((queue_t**)&taskQueue, (queue_t*)task);
  task->queue = &taskQueue;
}

void task_yield () {
  
  queue_append((queue_t**)&taskQueue, (queue_t*)execTask);
  execTask->queue = &taskQueue;

  userTasks++;
  
  task_switch(&dispatcher);
}

task_t *scheduler(){
  /* 
  task_t *aux, *next;
  int prioMin = MAXPRIO + 1;
  
  aux = taskQueue;
  do{

    if(aux->prioDin < prioMin){
        next = aux;
        prioMin = aux->prioDin;
    }else if(aux->prioDin == prioMin){
        if(aux->prioEst < next->prioEst){
          next = aux;
        }
        next = aux;
        prioMin = aux->prioDin;
    }

    aux = aux->next;

  }while(aux != taskQueue);

  next->prioDin = next->prioEst;


  do{

    if(aux->prioDin>MINPRIO && aux->prioDin < MAXPRIO)
      aux->prioDin += TASKAGING;

    aux = aux->next;

  }while(aux != taskQueue);

  userTasks--;

  task_t* prox=(task_t*) queue_remove((queue_t**)next->queue, (queue_t*)next);

  
  return prox;
  */
  
  userTasks--;
  return (task_t*) queue_remove((queue_t**)&taskQueue, (queue_t*)taskQueue);
  
}

void dispatcher_body () {
   task_t *next;
   int i=0;
   while ( userTasks > 0 ){
      next = scheduler();
      next->queue = NULL;
      next->quantum = TICKS;
      if (next){
         task_switch (next) ;
      }
   }
   task_exit(0) ;
}

void task_setprio (task_t *task, int prio){
  if(task == NULL){
    execTask->prioEst = prio;
  }else{
    if(prio>=MINPRIO && prio<=MAXPRIO){
        task->prioDin = prio;
        task->prioEst = prio;
    }
  }
}

int task_getprio (task_t *task) {
  if(task == NULL)
    task = execTask;

  return task->prioEst;
}

void tick_count(){
  ticks++;
  if(task_id() != idDispacher){
    execTask->quantum--;
    execTask->processorTime++;
    if(execTask->quantum <= 0){
      task_yield();
    }
  }
}

unsigned int systime (){
  return ticks;
}
