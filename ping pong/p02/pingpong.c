//Pedro Henrique Belotto Frankiewicz
//RA 1189212

#include "datatypes.h"
#include "pingpong.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#define STACKSIZE 32768		/* tamanho de pilha das threads */

long int id = 0;
task_t mainTask, *exec, *ant;


void pingpong_init (){
    setvbuf (stdout, 0, _IONBF, 0) ;

    mainTask.tid = id;
    exec = &mainTask;
    id++;
}


int task_create (task_t *task,	void (*start_func)(void *),	 void *arg){
    char* stack;

    getcontext(&(task->context));

    stack = malloc(STACKSIZE);
    if (stack){
      task->context.uc_stack.ss_sp = stack ;
      task->context.uc_stack.ss_size = STACKSIZE;
      task->context.uc_stack.ss_flags = 0;
      task->context.uc_link = 0;
   }
   else{
      perror ("Erro na criação da pilha: ");
      exit (1);
   }
    makecontext(&(task->context), (void *)(*start_func), 1, arg);
    #ifdef DEBUG
      printf("task create: criou a tarefa %d\n", task->tid);
    #endif
    task->tid = id++;

    return task->tid;
}


void task_exit (int exitCode){
    #ifdef DEBUG
      printf("task_exit: encerrando tarefa %d\n", exec->tid);
      #endif
    task_switch(&mainTask);
}


int task_switch (task_t *task){
    ant = exec;
    exec = task;
    #ifdef DEBUG
      printf("task_switch: trocando contexto %d -> %d\n", ant->tid, exec->tid);
    #endif
    if(swapcontext(&(ant->context), &(task->context))<0){
        exec = ant;
        return -1;
    }
    return 0;
}


int task_id (){
    return exec->tid;
}
