#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Process.h"
#include "Queue.h"

using namespace std;
Queue::Queue() {}// Empty constructor

/**
*pop()
*Returns the first element of the queue, then removed the element
*
*/
Process* Queue::pop() {
	if (length > 0) {// If the queue is not empty
		Process* out = this->first->value;//get the value of the first object
		this->first = this->first->next;//remove the first queue object
		this->length --;
		return out;
	}
	return NULL;// Return null if there is nothing
}
/**
*push(Process*)
*Takes in a pointer to a Process obj and puts it in the back of the queue
*
*/
void Queue::push(Process* p) {
	if(this->length > 0) {// If there are objects in the queue
		this->last->next = new qObj(p, NULL); // Add the new obj
		this->last = this->last->next;
		length ++;
	}
	else {
		this->first = new qObj(p, NULL);// Add the new obj
		this->last = this->first;
		length++;
	}
}
