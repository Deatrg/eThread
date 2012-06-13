typedef struct _semaphore{
	int value;
}semaphore;

void semaphore_create();
void semaphore_wait();
void semaphore_signal();
