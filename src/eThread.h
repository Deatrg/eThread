#ifndef ETHREAD
#define ETHREAD

//C++ Headers
#include <queue>
//C Headers
#include <signal.h>
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
}eThread;

int 	eThread_create(eThread*, void(*)(void), int);
int 	eThread_setQuantum(int);
void 	eThread_yield(void);
void 	eThread_exit(void);
void 	eThread_init(void);
eThread* getRunningThread(void);

#endif
