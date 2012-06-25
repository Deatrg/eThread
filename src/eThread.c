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
	target->context.uc_link = &idleThread.context;
	sigemptyset(&(target->context.uc_sigmask));
	makecontext(&target->context, function, 0, NULL);
	printf("Made Thread: %d || Context Pointer: %ld\n", target->threadID, &target->context);
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

void scheduler(void){
	swapcontext(&runningThread->context, &idleThread.context);
}

void setup(void){
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
	setitimer(ITIMER_REAL, &tval, 0);
	puts("Swap");
	runningThread = runQueue;
	swapcontext(&mainThread.context, &runningThread->context);
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
				puts("NULL");
				runningThread = runQueue;
			}
		}while(runningThread->state != RUNNABLE && runningThread != oldThread);
		eThread* temp;
		for(temp = runQueue; temp->next != NULL; temp = temp->next){
			printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
		}
		printf("ID: %d|| State: %d \n", temp->threadID, temp->state);
		if(runningThread == oldThread && runningThread->state != RUNNABLE){
			//No Runnable Threads
			swapcontext(&(oldThread->context), &mainThread.context);
		}
		else{
			//runningThread->state = RUNNING;
			swapcontext(&(oldThread->context), &(runningThread->context));
		}
	}
}
