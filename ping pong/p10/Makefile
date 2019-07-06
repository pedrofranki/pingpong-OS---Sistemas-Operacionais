TARGET = pingpong-semaphore
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): semaphore.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) semaphore.o pingpong.o queue.o

semaphore.o: pingpong-semaphore.c
	$(CC) -c pingpong-semaphore.c -o semaphore.o

pingpong.o: pingpong.c
	$(CC) -c pingpong.c -o pingpong.o

queue.o: queue.c
	$(CC) -c queue.c -o queue.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)
