TARGET = pingpong-preempcao
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): preempcao pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) preempcao pingpong queue

preempcao: pingpong-preempcao.c
	$(CC) -c pingpong-preempcao.c -o preempcao

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
