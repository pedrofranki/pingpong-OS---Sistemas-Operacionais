TARGET = pingpong-dispatcher
LIBS =
CC = gcc
FLAGS = -Wall
CQUEUE = 	queue.c
QUEUE = queue
CPING = pingpong.c


all: $(TARGET)
	@echo "Concluido"

$(TARGET): dispatcher pingpong queue
	$(CC) $(FLAGS) -o $(TARGET) dispatcher pingpong queue $(FLAGS)

dispatcher: pingpong-dispatcher.c
	$(CC) -c pingpong-dispatcher.c -o dispatcher $(FLAGS) 

pingpong: pingpong.c
	$(CC) -c pingpong.c -o pingpong $(FLAGS)

queue: queue.c
	$(CC) -c queue.c -o queue $(FLAGS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
