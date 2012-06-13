typedef struct _semaphore{
	int value;
}semaphore;

int semaphore_create(semaphore*, int);
int semaphire_destory(semaphore*);
int semaphore_wait(semaphore*);
int semaphore_signal(semaphore*);
