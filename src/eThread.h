#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>

#ifndef ETHREAD
#define ETHREAD
typedef struct _eThread{
	ucontext_t context;
	int threadID;
	int state;
	struct _eThread* next;
}eThread;
#endif

enum STATES{
	RUNNING,
	RUNNABLE,
	BLOCKED,
	EXIT
};

static eThread* runQueue;	//Points to head of runQueue
static eThread* runningThread;	//Points to currently running thread in the runQueue
static eThread* mainThread;	//Points back to the main thread of execution

static int timeQuantum;

int eThread_create(eThread*, void(*)(void), int);
int eThread_destroy(void);
int eThread_yield(void);
int eThread_setQuantum(int);
static void scheduler(int);
