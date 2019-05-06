TARGET = pingpong-tasks
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c

all: pingpong-tasks1 pingpong-tasks2 pingpong-tasks3 cont
	@echo "Concluido"

pingpong-tasks1: task1 pingpong
	gcc $(FLAGS) -o pingpong-tasks1 task1 pingpong

pingpong-tasks2: task2 pingpong
	gcc $(FLAGS) -o pingpong-tasks2 task2 pingpong

pingpong-tasks3: task3 pingpong
	gcc $(FLAGS) -o pingpong-tasks3 task3 pingpong

cont: context pingpong
	gcc $(FLAGS) -o cont context pingpong

task1: pingpong-tasks1.c datatypes.h pingpong.h
	gcc -c pingpong-tasks1.c -o task1

task2: pingpong-tasks2.c datatypes.h pingpong.h
	gcc -c pingpong-tasks2.c -o task2

task3: pingpong-tasks3.c datatypes.h pingpong.h
	gcc -c pingpong-tasks3.c -o task3

context: contexts.c datatypes.h pingpong.h
	gcc -c contexts.c -o context

pingpong: $(CPING) datatypes.h pingpong.h
	gcc -o pingpong -c $(CPING) $(FLAGS)

clean:
	rm -f *.o
