TARGET = pingpong-join
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): join pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) join pingpong queue -lm

join: pingpong-join.c
	$(CC) -c pingpong-join.c -o join

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
