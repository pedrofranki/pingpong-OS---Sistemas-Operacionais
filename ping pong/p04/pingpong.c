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

void dispatcher_body ();
task_t *scheduler();

long int id=0, userTasks=0;
task_t mainTask, //Tarefa Main
      *execTask,//tarefa em execução
      *ant,
      *taskQueue;// fila de tarefas prontas
task_t dispatcher;


void pingpong_init (){
    setvbuf (stdout, 0, _IONBF, 0) ;

    mainTask.tid = id++;

    execTask = &mainTask;
    task_create(&dispatcher, dispatcher_body, NULL);

}


int task_create (task_t *task,	void (*start_func)(void *),	 void *arg){
    char* stack;
    task->tid = id++;

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
    #ifdef DEBUG
      printf("task_switch: trocando contexto %d -> %d\n", ant->tid, taskExec->tid);
    #endif
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
      execTask->state = 's';
    }else{
      queue_remove((queue_t**)task->queue, (queue_t*)task);
      queue_append((queue_t**)queue, (queue_t*)task);
      execTask->state = 's';
      userTasks--;
      task->queue = queue;
    }
  }
}

void task_resume (task_t *task) {
  queue_remove((queue_t**)task->queue, (queue_t*)task);
  queue_append((queue_t**)&taskQueue, (queue_t*)task);
  task->queue = &taskQueue;
  task->state = 'r';
}

void task_yield () {
  if(execTask->tid != 0){
    queue_append((queue_t**)&taskQueue, (queue_t*)execTask);
    execTask->queue = &taskQueue;
    userTasks++;
  }
  task_switch(&dispatcher);
}

task_t *scheduler(){
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
  next->prioDin += TASKAGING;

  aux = taskQueue;
  int i=0;
  do{

    if(aux->prioDin>MINPRIO && aux->prioDin < MAXPRIO)
      aux->prioDin += TASKAGING;

    aux = aux->next;

  }while(aux != taskQueue);

  userTasks--;

  task_t* prox=(task_t*) queue_remove((queue_t**)next->queue, (queue_t*)next);

  return prox;
}

void dispatcher_body () // dispatcher é uma tarefa
{
   task_t *next;
   int i=0;
   while ( userTasks > 0 ){

      next = scheduler();

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
}

int task_getprio (task_t *task) {
  if(task == NULL)
    task = execTask;

  return task->prioEst;
}
