test : test.c libeThread.a
	g++ -Wall -o test -g test.c libeThread.a

libeThread.a : eThread.o semaphore.o
	ar rcs libeThread.a eThread.o semaphore.o

semaphore.o : semaphore.h semaphore.cpp
	g++ -c -Wall semaphore.cpp

eThread.o : eThread.h eThread.cpp
	g++ -g -Wall -c eThread.cpp

clean:
	rm *\.[a,o]
