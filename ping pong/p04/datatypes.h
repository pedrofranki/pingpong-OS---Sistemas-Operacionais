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
  char state;
  int prioEst;
  int prioDin;
} task_t ;

// estrutura que define um semáforo
typedef struct
{
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
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
