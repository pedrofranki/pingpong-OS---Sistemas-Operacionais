TARGET = pingpong-join
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): contab pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) contab pingpong queue

contab: pingpong-join.c
	$(CC) -c pingpong-join.c -o contab

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
