#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Process.h"

/**
*Process::Process(char* name, char* priority, char* arrival)
*	name: cstring that contains the name
*	priority: cstring that contains the priority
*	arrival: cstring that contains the arrival time
*/
Process::Process(char* name, char* priority, char* arrival) {
	this->ProcessName = strdup(name);
	this->Priority = atoi(priority);
	this->ArrivalTime = atoi(arrival);
}
// Empty constructor
Process::Process(){}
