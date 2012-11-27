#ifndef SEMAPHORE
#define SEMAPHORE

#include <queue>
#include "eThread.h"

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
