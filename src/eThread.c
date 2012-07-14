#include "eThread.h"

int eThread_create(eThread* target, void(*function)(void) , int stackSize){
	static int numThreads = 0;
	if(runQueue == NULL){
		runQueue = target;
	}
	else{
		eThread* temp;
		for(temp = runQueue; temp->next != NULL; temp = temp->next);
		temp->next = target;
	}
	getcontext(&target->context);
	target->threadID = numThreads++;
	target->state = RUNNABLE;
	target->next = NULL;
	target->context.uc_stack.ss_sp = calloc(stackSize, sizeof(char));
	target->context.uc_stack.ss_size = stackSize;
	target->context.uc_link = &idleContext;//Return to idle thread on exit
	sigemptyset(&(target->context.uc_sigmask));
	makecontext(&target->context, function, 0, NULL);
	printf("Made Thread: %d || Context Pointer: %ld\n", target->threadID, (long) &target->context);
	return 0;
}

void eThread_exit(void){
	runningThread->state = EXIT;
	raise(SIGALRM);
}

int eThread_yield(void){
	return raise(SIGALRM);
}

int eThread_setQuantum(int newValue){
	return (timeQuantum = newValue);
}

void scheduler(int value){
	swapcontext(&runningThread->context, &idleContext);
}

void eThread_init(void){
	//Set Handler
	struct sigaction action;
	action.sa_handler = scheduler;
	sigaction(SIGALRM, &action, NULL);
	//Set Timer
	struct itimerval tval;
	tval.it_interval.tv_sec = 0;
	tval.it_interval.tv_usec = timeQuantum;
	tval.it_value.tv_sec = 0;
	tval.it_value.tv_usec = timeQuantum;
	runningThread = runQueue;

	//Setup idleContext
	getcontext(&idleContext);
	idleContext.uc_stack.ss_sp = calloc(IDLESTACK, sizeof(char));
	idleContext.uc_stack.ss_size = IDLESTACK;
//	idleContext.uc_link = &mainContext;//Return to main thread on exit
	sigemptyset(&idleContext.uc_sigmask);
	makecontext(&idleContext, idleThread, 0, NULL);
	//Swap to idle
	setitimer(ITIMER_REAL, &tval, 0);
	swapcontext(&mainContext, &idleContext);
	//Turn Timer Off upon returning to Main
	tval.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &tval, 0);
	puts("Return");
	eThread* temp;
	for(temp = runQueue; temp->next != NULL; temp = temp->next){
		printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
	}
	printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
	return;
}

void idleThread(void){
	while(1){
		eThread* oldThread = runningThread;
		if(runningThread->state != EXIT || runningThread->state != BLOCKED){
			runningThread->state = RUNNABLE;
		}
		do{
			runningThread = runningThread->next;
			if(runningThread == NULL){
				runningThread = runQueue;
			}
		}while(runningThread->state != RUNNABLE && runningThread != oldThread);
		eThread* temp;
		for(temp = runQueue; temp->next != NULL; temp = temp->next){
		//	printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
		}
		//printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
		if(runningThread == oldThread && runningThread->state != RUNNABLE){
			//No Runnable Threads
			break;//Let idleContext return and use uc_link to return to main
		}
		else{
			runningThread->state = RUNNING;
			swapcontext(&idleContext, &(runningThread->context));
			//runningThread->state = EXIT;//Indicate context has returned
			//Need to decide whether thread returned of was swapped out
		}
	}
	puts("Returning to Main");
	return;
}
