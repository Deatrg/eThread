#include "eThread.h"

typedef struct _waitQueue{
	struct _waitQueue* next;
	eThread* queued;
}waitQueue;

typedef struct _semaphore{
	int value;
	waitQueue* blocked;
}semaphore;

int semaphore_create(semaphore*, int);
int semaphire_destory(semaphore*);
int semaphore_wait(semaphore*);
int semaphore_signal(semaphore*);
