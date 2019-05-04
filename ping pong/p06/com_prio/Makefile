TARGET = pingpong-contab-prio
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): contabPrio pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) contabPrio pingpong queue

contabPrio: pingpong-contab-prio.c
	$(CC) -c pingpong-contab-prio.c -o contabPrio

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong

queue: queue.c
	$(CC) -c queue.c -o queue

clean:
	-rm -f *.o
	-rm -f $(TARGET)
