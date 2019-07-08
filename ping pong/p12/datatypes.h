//Pedro Henrique Belotto Frankiewicz
//RA 1189212

#ifndef __DATATYPES__
#define __DATATYPES__
#include <ucontext.h>

// Estrutura que define uma tarefa
typedef struct task_t{
  struct task_t *prev, *next;
  int tid;
  ucontext_t context;
  struct task_t** queue;
  struct task_t* susQueue;
  char state;
  int quantum;
  int prioEst;
  int prioDin;
  int wait;
  int exitCode;
  unsigned int creationTime;
  unsigned int exitTime;
  unsigned int processorTime;
  unsigned int executionTime;
  unsigned int sleepTime;
  int activations;
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  task_t *queue;
  int id;
  int value;
  int status;
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  task_t *bQueue;
  int max;
  int n;
  int status;
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  void *msgQueue;
  int maxMsg;
  int sizeMsg;
  int countMsg;
  int index;
  int status;
  semaphore_t s_item;
  semaphore_t s_fila;
  semaphore_t s_vaga;
} mqueue_t ;

#endif
