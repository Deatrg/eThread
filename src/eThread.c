#include "eThread.h"

using namespace std;

int eThread_create(eThread* target, void(*function)(void) , int stackSize){
	static int numThreads = 0;
	getcontext(&target->context);
	target->threadID = numThreads++;
	target->state = RUNNABLE;
	target->next = NULL;
	target->context.uc_stack.ss_sp = calloc(stackSize, sizeof(char));
	target->context.uc_stack.ss_size = stackSize;
	target->context.uc_link = &idleContext;//Return to idle thread on exit
	sigemptyset(&(target->context.uc_sigmask));
	makecontext(&target->context, function, 0, NULL);
	runQueue.push(target);
	return 0;
}

void eThread_exit(void){
	runQueue.front()->state = EXIT;
	raise(SIGALRM);
}

int eThread_yield(void){
	return raise(SIGALRM);
}

int eThread_setQuantum(int newValue){
	return (timeQuantum = newValue);
}

void scheduler(int value){
	if(runQueue.front()->state != EXIT || runQueue.front()->state != BLOCKED){//Check if eThread_exit was called or thread was blocked.
		runQueue.front()->state = RUNNABLE;//Indicates was swapped not returned.
	}
	swapcontext(&runQueue.front()->context, &idleContext);
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
	//Setup idleContext
	getcontext(&idleContext);
	idleContext.uc_stack.ss_sp = calloc(IDLESTACK, sizeof(char));
	idleContext.uc_stack.ss_size = IDLESTACK;
	idleContext.uc_link = &mainContext;//Return to main thread on exit
	sigemptyset(&idleContext.uc_sigmask);
	makecontext(&idleContext, idleThread, 0, NULL);
	//Swap to idleContext
	setitimer(ITIMER_REAL, &tval, 0);
	tval.it_value.tv_usec = 0;
	swapcontext(&mainContext, &idleContext);
	//Turn timer off upon returning to Main
	setitimer(ITIMER_REAL, &tval, 0);
	return;
}

void idleThread(void){
	while(1){
		eThread* oldThread = runQueue.front();
		runQueue.push(oldThread);
		runQueue.pop();
		while(runQueue.front()->state != RUNNABLE && runQueue.front() != oldThread){
			runQueue.push(runQueue.front());
			runQueue.pop();
		}
		if(runQueue.front() == oldThread && runQueue.front()->state != RUNNABLE){
			break;//No RUNNABLE threads left. Let idleContext return and use uc_link to return to main context.
		}
		else{
			runQueue.front()->state = RUNNING;
			swapcontext(&idleContext, &(runQueue.front()->context));
		}
		if(runQueue.front()->state == RUNNING){
			runQueue.front()->state = EXIT;//Thread returned
		}
	}
	return;
}
