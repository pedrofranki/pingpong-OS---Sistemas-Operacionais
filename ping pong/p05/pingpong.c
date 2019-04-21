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

void dispatcher_body ();
task_t *scheduler();

long int id=0, userTasks=0;
task_t mainTask, *execTask, *ant, *taskQueue;
task_t dispatcher;




void pingpong_init (){
    setvbuf (stdout, 0, _IONBF, 0) ;

    mainTask.next = NULL;
    mainTask.prev = NULL;
    mainTask.tid = id++;

    execTask = &mainTask;
    task_create(&dispatcher, dispatcher_body, NULL);

}


int task_create (task_t *task,	void (*start_func)(void *),	 void *arg){
    char* stack;
    task->tid = id++;
    task->next = NULL;
    task->prev = NULL;
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
      //printf("aaa\n");
      queue_append((queue_t**)&taskQueue, (queue_t*)task);
      userTasks++;
      task->queue = &taskQueue;
      task->state = 'e';
    }

    task->tid = id;
    id++;
#ifdef DEBUG
    printf("task_create: task %d criada.\n", task->tid);
#endif

    return task->tid;
}


void task_exit (int exitCode){
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

  queue_remove((queue_t**)task->queue, (queue_t*)task);

  queue_append((queue_t**)queue, (queue_t*)task);
  task->state = 's';
  userTasks--;
  task->queue = queue;
}

void task_resume (task_t *task) {
  queue_remove((queue_t**)task->queue, (queue_t*)task);
  queue_append((queue_t**)&execTask, (queue_t*)task);
  task->queue = &execTask;
  task->state = 'p';
}

void task_yield () {
  if(execTask->tid != 0){
    queue_append((queue_t**)&taskQueue, (queue_t*)execTask);
    execTask->queue = &taskQueue;
    execTask->state = 'e';
    userTasks++;
  }
  task_switch(&dispatcher);
}

task_t *scheduler(){
  task_t *aux, *next;
  int prioMin = MAXPRIO + 1;

  aux = taskQueue;
//  printf("%d\n",queue_size(taskQueue));
  do{
    //printf("carlos\n");
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
//printf("awdlos\n");
  next->prioDin = next->prioEst;
  next->prioDin += TASKAGING;

  aux = taskQueue;
  int i=0;
  do{
    //printf("%d\n", i++);
    if(aux->prioDin>MINPRIO && aux->prioDin < MAXPRIO)
      aux->prioDin += TASKAGING;

    aux = aux->next;
    //printf("aaaa\n");
  }while(aux != taskQueue);
  //printf("www\n");
  userTasks--;
  //printf("qqqqqaa\n");
  task_t* prox=(task_t*) queue_remove((queue_t**)next->queue, (queue_t*)next);
  //printf("adawd\n");
  return prox;
}

void dispatcher_body () // dispatcher é uma tarefa
{
   task_t *next;
   int i=0;
   while ( userTasks > 0 ){
      //printf("\t\t%d\n", i++);
      next = scheduler();
      //printf("adawd\n");
      next->queue = NULL;
      if (next){
         task_switch (next) ; // transfere controle para a tarefa "next"
      }
   }
   task_exit(0) ; // encerra a tarefa dispatcher
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
  //printf("%d\n", prio);
}

int task_getprio (task_t *task) {
  if(task == NULL)
    task = execTask;

  return task->prioEst;
}
