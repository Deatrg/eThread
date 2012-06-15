#include "eThread.h"

int eThread_create(eThread* target, void(*function)(void) , int stackSize){
	static int numThreads = 0;
	if(runningThread == NULL){
		runningThread = target;
	}
	else{
		runningThread->next = target;
	}
	getcontext(&(target->context));
	target->threadID = numThreads++;
	target->state = RUNNABLE;
	target->next = NULL;
	target->context.uc_stack.ss_sp = calloc(stackSize, sizeof(char));
	target->context.uc_stack.ss_size = stackSize;
	target->context.uc_link = &(mainThread->context);
	sigemptyset(&(target->context.uc_sigmask));
	makecontext(&(target->context), function, 0, NULL);
	return 0;
}

int eThread_destory(void){
	return 0;
}
