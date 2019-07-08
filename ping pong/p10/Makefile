TARGET1 = pingpong-prodcons
TARGET2 = pingpong-racecond
TARGET3 = pingpong-semaphore
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET1): prodcons.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) prodcons.o pingpong.o queue.o -lm

$(TARGET2): racecond.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) racecondsleep.o pingpong.o queue.o -lm

$(TARGET3): semaphore.o pingpong.o queue.o
	$(CC) $(FLAGS) -o $(TARGET) semaphore.o pingpong.o queue.o -lm

semaphore.o: pingpong-semaphore.c
	$(CC) -c pingpong-semaphore.c -o semaphore.o

racecond.o: pingpong-racecond.c
	$(CC) -c pingpong-racecond.c -o racecond.o

prodcons.o: pingpong-prodcons.c
	$(CC) -c pingpong-prodcons.c -o prodcons.o

pingpong.o: pingpong.c
	$(CC) -c pingpong.c -o pingpong.o

queue.o: queue.c
	$(CC) -c queue.c -o queue.o

clean:
	-rm -f *.o
	-rm -f $(TARGET)
