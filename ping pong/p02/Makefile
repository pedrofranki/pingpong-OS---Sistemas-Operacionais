all: saida

saida: pingpong-tasks1.o pingpong.o
	gcc -o saida pingpong-tasks1.o pingpong.o

pingpong-tasks1.o: pingpong-tasks1.c pingpong.h
	gcc -o pingpong-tasks1.o pingpong-tasks1.c -c -W -Wall

pingpong.o: pingpong.c pingpong.h
	gcc -o pingpong.o pingpong.c -c -W -Wall

clean:
	rm -f *.o
