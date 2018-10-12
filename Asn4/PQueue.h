#ifndef PQUEUE
#define PQUEUE
#include "Process.h"
/**
*Object used to hold PQueue objects
*/
struct pObj {
	Process* value = NULL;
	pObj* next;
	pObj(Process* p, pObj* _next){value=p;next=_next;}
};
/**
*Queue implementation that sorts by priority
*/
class PQueue {
	public:
		PQueue();
		pObj* first = NULL;
		pObj* last = NULL;
		Process* pop();
		void push(Process* p);
		int length = 0;
};
#endif
