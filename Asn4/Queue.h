#ifndef QUEUE
#define QUEUE
#include "Process.h"
/**
*qObj is an object used to keep track of Queue objects
*qObg(Process* Object-Being-Stored, qObj* Next-Object-In-Queue);
*
*/
struct qObj {
	Process* value = NULL;//qObj stored information
	qObj* next;// next qObj in the queue
	qObj(Process* p, qObj* _next){value=p;next=_next;}
};
/*
*Simple implementation of a queue
*
*/
class Queue {
	public:
		Queue();
		qObj* first = NULL;
		qObj* last = NULL;
		Process* pop();
		void push(Process* p);
		int length = 0;
};
#endif
