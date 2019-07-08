TARGET = pingpong-barrier
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): barrier.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) barrier.o pingpong.o queue.o -lm

barrier.o: pingpong-barrier.c
	$(CC) -c pingpong-barrier.c -o barrier.o

pingpong.o: pingpong.c
	$(CC) -c pingpong.c -o pingpong.o

queue.o: queue.c
	$(CC) -c queue.c -o queue.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)
