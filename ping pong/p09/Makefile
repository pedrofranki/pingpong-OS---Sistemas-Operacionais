TARGET = pingpong-sleep
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): sleep pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) sleep pingpong queue -lm

sleep: pingpong-sleep.c
	$(CC) -c pingpong-sleep.c -o sleep

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
