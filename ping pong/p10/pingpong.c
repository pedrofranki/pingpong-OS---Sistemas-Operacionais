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
//int i=0;
long int id = 0, userTasks=0;
task_t mainTask, *execTask, *ant, *taskQueue, *suspendQueue, *sleepQueue;
task_t dispatcher;
int ticks = 0, preempcao = 1, sId=0;
int idDispacher;
struct sigaction action;
struct itimerval timer;

void pingpong_init (){
    setvbuf (stdout, 0, _IONBF, 0) ;

    mainTask.tid = id++;

    execTask = &mainTask;
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
    //printf("aaaa\n");
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
    //printf("%d", task->tid);
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
    printf("Task %d exit: execution time %d ms, processor time %d ms, %d activations\n", execTask->tid,
    execTask->exitTime - execTask->creationTime, execTask->processorTime, execTask->activations);
    
    #ifdef DEBUG
        printf("task_exit: encerrando task %d.\n", taskExec->tid);
    #endif
    if(queue_size((queue_t*)execTask->susQueue)>0){
        //printf("aaaaaaaa\n");
        task_resume(execTask->susQueue);
    }
    execTask->state = 'f';
    execTask->exitCode = exitCode;

 

    if(execTask == &dispatcher){
      task_switch(&mainTask);
    }else{
      dispatcher.activations++;
      task_switch(&dispatcher);
    }
      
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
    if(task == NULL ){
      //printf("s1\n");
      queue_append((queue_t**)queue, (queue_t*)execTask);
      execTask->queue= queue;
      //execTask->state = 's';
      task_switch(&dispatcher);
    }else{   
      if(task->queue != NULL)
        queue_remove((queue_t**)(task->queue), (queue_t*)task);
      //printf("s2\n");
      queue_append((queue_t**)queue, (queue_t*)task);
      
      task->queue = queue;

    }
  }
  execTask->state = 's';
}

void task_resume (task_t *task) {

  if(task->queue!=NULL){
    ///printf("resmma\n");
    queue_remove((queue_t**)(task->queue), (queue_t*)task);
  }
  //printf("%d\n", task->tid);
  queue_append((queue_t**)&taskQueue, (queue_t*)task);
  userTasks++;
  task->state = 'r';
  task->queue = &taskQueue;

}

int task_join (task_t *task) {
  if(task == NULL){
    return -1;
  }else if(task->state =='f'){
    return task->exitCode;
  }else{
    task_suspend(NULL, &task->susQueue);
    task_switch(&dispatcher);
    return task->exitCode;
  }
}

int sem_create (semaphore_t *s, int value) {
  if(s == NULL){
    return -1;
  }
  sId++;
  
  s->id = sId; 
  s->queue = NULL;
  s->value = value;
  s->status = 1;

  return 0;

}

// requisita o semáforo
int sem_down (semaphore_t *s) {
  preempcao = 0;
  
  if(s == NULL || s->status==0){
    preempcao = 1;
    return -1;
  }
  
  s->value--;
  if(s->value<0){
    preempcao = 1;

    task_suspend(execTask, &(s->queue));
    task_switch(&dispatcher);
    //task_yield();
  }
  preempcao = 1;
  return 0;
}

// libera o semáforo
int sem_up (semaphore_t *s) {
  preempcao =0 ;
  if(s == NULL || s->status==0){
    preempcao = 1;
    return -1;
  }
  s->value++;
  if(s->value<=0){
    task_resume(s->queue);
  }
  preempcao = 1;
  return 0;

}

// destroi o semáforo, liberando as tarefas bloqueadas
int sem_destroy (semaphore_t *s) {
  preempcao =0 ;
  if(s == NULL || s->status==0){
    preempcao = 1;
    return -1;
  }

  if(s->status !=0){
    s->status = 0;
    while(s->queue != NULL){
      preempcao = 1;
      task_resume(s->queue);
    } 
  }
  preempcao = 1;
  return 0;
  
}

void task_yield () {
  if(execTask->state != 's'){ 
    queue_append((queue_t**)&taskQueue, (queue_t*)execTask);
    userTasks++;
    execTask->queue = &taskQueue;
    execTask->state = 'r';
  }

  
  dispatcher.activations++;
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
  //printf("%ld\n", userTasks);
  return (task_t*) queue_remove((queue_t**)&taskQueue, (queue_t*)taskQueue);
  
}

void dispatcher_body () {
   task_t *next;
   while (userTasks > 0 || sleepQueue!=NULL){
     
      if(taskQueue != NULL){
        next = scheduler();
        userTasks = queue_size((queue_t*)taskQueue);
        next->queue = NULL;
        next->quantum = TICKS;
        next->activations++;
        if (next){
          task_switch (next) ;
        }
      }
      if(sleepQueue != NULL){
        task_t *aux = sleepQueue, *rem;
        
        do{
          rem = aux;
          
          if(rem->sleepTime <= systime()){
            
            aux=aux->next;
           // printf("sl\n");
            queue_append((queue_t**)&taskQueue,queue_remove((queue_t**)&sleepQueue,(queue_t*)rem));
            //task_resume(rem);
            userTasks++;

          }else{
            aux = aux->next;
          }
        }while(aux!=sleepQueue && sleepQueue != NULL);
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
    if(execTask->quantum <= 0 && preempcao == 1){
      task_yield();
    }
  }
}

unsigned int systime (){

  return ticks;
}

void task_sleep (int t) {
  execTask->sleepTime = systime() + t*1000;
  queue_append((queue_t**)&sleepQueue, (queue_t*)execTask);
  execTask->queue = &sleepQueue;

  task_switch(&dispatcher);
}

