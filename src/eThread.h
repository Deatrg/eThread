#include <signal.h>
#include <ucontext.h>

typedef struct _eThread{
	ucontext_t context;
	int threadID;
	int state;
}eThread;

static int timeQuantum;

void eThread_create(void(*)(void), int);
void eThread_destroy(void);
