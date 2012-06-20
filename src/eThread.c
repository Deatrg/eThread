#include "eThread.h"

int eThread_create(eThread* target, void(*function)(void) , int stackSize){
	static int numThreads = 0;
	if(runQueue == NULL){
		runQueue = target;
	}
	else{
		for(eThread* temp = runQueue; temp->next != NULL; temp = temp->next);
		temp->next = target;
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
	runningThread->state = EXIT;
	raise(SIGALRM);
	return 0;
}

int eThread_yield(void){
	raise(SIGALRM);
	return 0;
}

int eThread_setQuantum(int newValue){
	timeQuantum = newValue;
	return 0;
}

static void secheduler(int value){
	int exitToMain;
	eThread* oldThread = runningThread;
	if(runningThread->state != EXIT || runningThread->state != BLOCKED){
		runningThread->state = RUNNABLE;
	}
	do{
		runningThread = runningThread->next;
		if(runningThread->next == NULL){
			runningThread = runQueue;
		}
	}while(runningThread->state != RUNNABLE && (exitToMain = (runningThread != oldThread)));
	if(exitToMain != 0 && runningThread->state != RUNNABLE){
		//No Runnable Threads
		swapcontext(&(oldThread->context), &mainThread->context);
	}
	else{
		runningThread->state = RUNNING;
		swapcontext(&(oldThread->context), &(runningThread->context));
	}
	return;
}
