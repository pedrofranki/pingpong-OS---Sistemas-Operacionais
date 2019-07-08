TARGET = pingpong-mqueue
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): mqueue.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) mqueue.o pingpong.o queue.o -lm

mqueue.o: pingpong-mqueue.c
	$(CC) -c pingpong-mqueue.c -o mqueue.o

pingpong.o: pingpong.c
	$(CC) -c pingpong.c -o pingpong.o

queue.o: queue.c
	$(CC) -c queue.c -o queue.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)
