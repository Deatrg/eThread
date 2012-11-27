#include "../src/eThread.h"
#include "../src/semaphore.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

static semaphore counter_mutex(1);
int counter = 0;
double result = 0.0;

void func1(void);
void func2(void);
void function(void);

int main(void){
	eThread_setQuantum(100);
	eThread eThread_1;
	eThread eThread_2;
	eThread_create(&eThread_1, function, 4*4096);
	eThread_create(&eThread_2, function, 4*4096);
	eThread_init();
	printf("Counter:%d||Value:%f\n", counter, result);
	return 0;
}

void func1(void){
	for(int i = 0; i < 1000; i++){
		printf("1:%d\n", i);
		counter++;
	}
//	eThread_exit();
}

void func2(void){
	for(int i = 0; i < 1000; i++){
		printf("2:%d\n",i);
		counter++;
	}
//	eThread_exit();
}


void function(void){
	for(int i = 0; i < 5; i++){
		counter_mutex.wait();
		for(int j = 0; j < 1000; j++){
//			cout << runQueue.front()->threadID << endl;
			result = result + sin(counter) * tan(counter);
		}
		counter++;
		counter_mutex.signal();
	}
}
