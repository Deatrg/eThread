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

#define IDLESTACK	4*1024

typedef struct _eThread{
	int threadID;
	int state;
	ucontext_t context;
	struct _eThread* next;
}eThread;

static eThread*		runQueue;	//Points to head of runQueue
static eThread* 	runningThread;	//Points to currently running thread in the runQueue
static ucontext_t	mainContext;	//Context of the main thread of execution
static ucontext_t	idleContext;	//Does All Schduling

static int timeQuantum;

int 	eThread_create(eThread*, void(*)(void), int);
void 	eThread_exit(void);
int 	eThread_yield(void);
int 	eThread_setQuantum(int);
void 	eThread_init(void);
void	scheduler(int);
void	idleThread(void);
#endif
