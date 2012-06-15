libeThread.a : eThread.o semaphore.o
	ar rcs libeThread.a eThread.o semaphore.o

semaphore.o : semaphore.h semaphore.c
	gcc -c semaphore.c

eThread.o : eThread.h eThread.c
	gcc -c eThread.c

clean:
	rm *\.[a,o]
