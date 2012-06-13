#include "semaphore.h"


int semaphore_create(semaphore* target, int value){
	target->value = value;
	return 0;
}

int semaphore_destroy(semaphore* target){
	return 0;
}

int semaphore_wait(semaphore* taget){
	return 0;
}

int semaphore_signal(semaphore* target){
	return 0;
}
