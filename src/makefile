test : test.c libeThread.a
	g++ -o test -g test.c libeThread.a

libeThread.a : eThread.o 
	ar rcs libeThread.a eThread.o

semaphore.o : semaphore.h semaphore.c
	g++ -c semaphore.c

eThread.o : eThread.h eThread.c
	g++ -g -Wall -c eThread.c

clean:
	rm *\.[a,o]
