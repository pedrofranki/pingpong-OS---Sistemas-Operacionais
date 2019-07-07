#include <stdio.h>
#include <stdlib.h>
#include "pingpong.h"
#include "queue.h"
#define TAMBUF 5
#define PRODUTORES 3
#define CONSUMIDORES 2
// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

typedef struct buffer{
    struct buffer *prev, *next;
    int item;
}buffer;

semaphore_t s_buffer , s_item , s_vaga;
buffer *buf;
task_t p1, p2, p3, c1, c2;

void produtor(void *arg){
    buffer *aux = (buffer*)malloc(sizeof(buffer));
    int item;
    //printf("dada\n");
    while(1){
        printf("awada\n");
        printf("%d\n", queue_size( (queue_t*)buf));
        task_sleep(1);
        item = random()%100;
        aux->item = item;
        sem_down(&s_item);
        sem_down(&s_buffer);
        printf("%d\n", queue_size( (queue_t*)buf));
        if(queue_size( (queue_t*)buf)< 5){
            printf("aadawdhahwd\n");
            queue_append((queue_t**)&buf, (queue_t*)aux);
        }
        sem_up(&s_buffer);
        sem_up(&s_item);
        printf("%s produziu %d\n", (char*)arg, item);
    }
    task_exit(0);

}


void consumidor(void *arg){
    buffer *item;
    printf("ada\n");
    while(1){
        printf("ada\n");
        sem_down(&s_item);
        sem_down(&s_buffer);
        if( queue_size((queue_t*)buf)>0 ){
            printf("aadawdddddddddhahwd\n");
            item = (buffer*) queue_remove( (queue_t**)&buf, (queue_t*)buf );

        }
        sem_up(&s_buffer);
        sem_up(&s_vaga);
        printf("\t\t\t %s consumiu %d\n",(char*)arg, item->item);
        task_sleep(1);
    }
    task_exit(0);
}

int main(){
    
    pingpong_init();
    
    sem_create(&s_item, 0);
    sem_create(&s_buffer, 1);
    sem_create(&s_vaga, 5);

    task_create(&p1, produtor,"p1");
    task_create(&p2, produtor, "p2");
    task_create(&p3, produtor,"p3");

    task_join(&p1);

    task_create(&c1, consumidor, "c1");
    task_create(&c2, consumidor,"c2");

    task_exit(0);
    exit(0);


}

