TARGET = pingpong-scheduler
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): scheduler pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) scheduler pingpong queue

scheduler: pingpong-scheduler.c
	$(CC) -c pingpong-scheduler.c -o scheduler

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
