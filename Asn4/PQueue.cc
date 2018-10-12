#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Process.h"
#include "PQueue.h"

using namespace std;
PQueue::PQueue() {}// Empty constructor
/**
*pop()
* returns the first element of the queue, then removes it
*/
Process* PQueue::pop() {
	if (length > 0) {// if there is somthing in the queue
		Process* out = this->first->value;// get the output
		this->first = this->first->next;
		this->length --;
		return out;//return
	}
	return NULL;
}
/**
*push(Process*)
*	takes a pointer to a process and puts it into the queue ordered by priority
*
*/
void PQueue::push(Process* p) {
	if(this->length > 0) {// if queue is not empty
		this->length ++;
		pObj* pre;
		pObj* point = this->first;
		if(point->value->Priority > p->Priority) {// check if the new obj is the lowest priority number
			this->first = new pObj(p, this->first);
			return;
		}
		pre = point;
		point = point->next;
		while(point != NULL) {// Look for where to put the new obj
			if(point->value->Priority > p->Priority) {
				pre->next = new pObj(p, point);
				return;
			}
			pre = point;
			point = point->next;
		}
		pre->next = new pObj(p, NULL);// if no placement was found, put it at the end
	}
	else {
		this->first = new pObj(p, NULL);// insert the first obj
		this->last = this->first;
		this->length ++;
	}
}
