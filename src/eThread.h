#ifndef ETHREAD
#define ETHREAD

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/time.h>

#define	RUNNING		0
#define	RUNNABLE	1
#define	BLOCKED		2
#define	EXIT		3

typedef struct _eThread{
	ucontext_t context;
	int threadID;
	int state;
	struct _eThread* next;
}eThread;

static eThread* runQueue;	//Points to head of runQueue
static eThread* runningThread;	//Points to currently running thread in the runQueue
static eThread  mainThread;	//Contains the context of the main thread of execution
static eThread	idleThread;	//Does All Schduling

static int 	timeQuantum;

int 	eThread_create(eThread*, void(*)(void), int);
void 	eThread_exit(void);
int 	eThread_yield(void);
int 	eThread_setQuantum(int);
void 	eThread_init(void);
void	scheduler(int);
#endif
