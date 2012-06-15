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

static eThread* runQueue;
static eThread* runningThread;
static eThread* mainThread;

static int timeQuantum;

int eThread_create(eThread*, void(*)(void), int);
int eThread_destroy(void);
