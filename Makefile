a.out: co.o server.o
		gcc server.o co.o -lm

server.o: server.c server.h
		gcc -c server.c -lm

co.o: co.c server.h
		gcc -c co.c -lm