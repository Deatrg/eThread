#ifndef SEMAPHORE
#define SEMAPHORE
#include "eThread.h"
#include <queue>

class semaphore{
private:
	int value;
	std::queue<eThread*> waitQueue;
public:
	semaphore(int);
	void wait(void);
	void signal(void);
};
#endif
