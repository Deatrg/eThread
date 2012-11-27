#ifndef ETHREAD
#define ETHREAD

//C++ Headers
#include <queue>
//C Headers
#include <signal.h>
#include <ucontext.h>
#include <sys/time.h>

#define IDLESTACK 4*1024

enum States {RUNNING, RUNNABLE, BLOCKED, EXIT};

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
