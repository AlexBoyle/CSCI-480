#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Process.h"
#include "Queue.h"
#include "PQueue.h"

using namespace std;
/**
* Prints ou the final values of a process
*
*terminate(Process* p, int time)
*	p: process being terminated
*	time: time the process was terminated
*/
void terminate(Process* p, int time) {
	cerr << "A process has terminated." << '\n';
        cerr << "Process ID:          " << p->ProcessID << '\n';
        cerr << "Name:                " << p->ProcessName << '\n';
        cerr << "Priority:            " << p->Priority << '\n';
        cerr << "Started at time      " << p->ArrivalTime  << " and ended at time " << time << '\n';
        cerr << "Total CPU time =     " << p->CPUTotal << " in " << p->CPUCount << " CPU bursts" << '\n';
        cerr << "Total Input time =   " << p->ITotal << " in " << p->ICount << " Input bursts" << '\n';
        cerr << "Total Output time =  " << p->OTotal << " in " << p->OCount << " Output bursts" << '\n';
        cerr << "Time spent waiting:  " << p->wait << "\n\n";
}
int main() {
	// Below are const vars
	const int MAX_TIME = 500;
	const int AT_ONCE = 5;
	const int QUEUE_SIZE = 20;
	const int ARRAY_SIZE = 10;
	const int HOW_OFTEN = 25;
	int tProcesses = 0;
	int idleTime = 0;
	// Below are the active process and queue tracking vars
	Process* Active = NULL;
	Process* IActive = NULL;
	Process* OActive = NULL;
	Queue* Entry = new Queue();
	PQueue* Ready = new PQueue();
	PQueue* Input = new PQueue();
	PQueue* Output = new PQueue();
	int Timer = 0;
	// Below are the file reading vars
	FILE *fp;
	fp = fopen("./data4.txt", "r");// Open file as read only
	char temp;
	char* buff = new char[500];// line buffer
	int i = 0;
	int pidC = 100;
	bool isNew = false;
	Process* p = NULL;// pointer to currently populating process
	while((temp = fgetc(fp)) != EOF) {//read one char from file
		if(temp == '\n') {// if it is a new line char
			isNew = !isNew; // swap the \n flag
			buff[i+1] = '\0';// add a null terminator to the buff
			i = 0;
			if (isNew) { //if this is the first line of a process
				int aName = strchr(buff, ' ') - buff;//get index of last char in name
				buff[aName] = '\0';
				int aPri = strchr(&buff[aName+1], ' ') - buff;//get index of last char in priority
				buff[aPri] = '\0';
				p = new Process(buff, &buff[aName + 2], &buff[aPri + 2]);// create a new process
			}
			else {// if this is the second line of a process
				char action = NULL;
				int index = 0;
				do {// while  there are history objects
					buff[index+5] = '\0';//get last index of action and null terminate it
					action = buff[index];//read the action char
					p->history[index/8] = new histObj(action, atoi(&buff[index+1]));// Create a new history obj
					index += 8;// go to the next element

				} while (action != 'N');//While we have not reached the terminate char
				pidC ++;// inc process id
				p->ProcessID = pidC;// set pid
				Entry->push(p);//add the process to the queue
			}
			if(strcmp(p->ProcessName, "STOPHERE") == 0 )// Check for read stop flag
				break;
		}
		else {// add char to buff
			buff[i] = temp;
			i ++;
		}
	}
	fclose(fp);// close file reader
	//Start of CPU loop
	cerr << "\nSimulation of Priority Scheduling\n\n";
	for(;Timer < MAX_TIME; Timer++) {// timer loop
		if(Active == NULL && IActive == NULL && OActive == NULL) {// if there are no active processes then idle
			idleTime ++;
		}
		if(Active != NULL) {// if there is an active process
			Active->history[Active->sub]->value --;// dec the hist value
			Active->CPUTotal ++;// add to the processes cpu time
			if(Active->history[Active->sub]->value == 0) {// if at the end of hist obj
				Active->sub ++;// inc the  sub
				if(Active->history[Active->sub]->type == 'I') {//if input
					Input->push(Active);// put in the input queue
					Active = NULL;
				}
				else if(Active->history[Active->sub]->type == 'O') {//if output
					Output->push(Active);//put in the output queue
					Active = NULL;
				}
				else if(Active->history[Active->sub]->type == 'N') {// if terminate
					terminate(Active, Timer);//kill process
					tProcesses ++;
					Active = NULL;
				}
			}
		}
		else if(Ready->length > 0) {// if there is no active process get one
			Active = Ready->pop();// get process off of ready queue
			Active->CPUCount ++;
			pObj* ptr = Ready->first;
			while(ptr != NULL) {// add wait time to every process in the queue
				ptr->value->wait ++;
				ptr = ptr->next;
			}
		}
		if(IActive != NULL) {// same process as in the above Active if
                        IActive->history[IActive->sub]->value --;
			IActive->ITotal ++;
                        if(IActive->history[IActive->sub]->value == 0) {
                                IActive->sub ++;
                                if(IActive->history[IActive->sub]->type == 'C') {
                                        Ready->push(IActive);
                                        IActive = NULL;
                                }
				 else if(IActive->history[IActive->sub]->type == 'O') {
                                        Output->push(IActive);
                                        IActive = NULL;
                                }
                                else if(IActive->history[IActive->sub]->type == 'N') {
                                        terminate(IActive, Timer);
					tProcesses ++;
                                        IActive = NULL;
                                }
                        }
                }
		else if(Input->length > 0) {// if there is no Input process, get one
			IActive = Input->pop();
			IActive->ICount ++;
			pObj* ptr = Input->first;
                        while(ptr != NULL) {
                                ptr->value->wait ++;
                                ptr = ptr->next;
                        }
		}
		if(OActive != NULL) {// same code as the Active if
                        OActive->history[OActive->sub]->value --;
			OActive->OTotal ++;
                        if(OActive->history[OActive->sub]->value == 0) {
                                OActive->sub ++;
                                if(OActive->history[OActive->sub]->type == 'I') {
                                        Input->push(OActive);
                                        Active = NULL;
                                }
                                else if(OActive->history[OActive->sub]->type == 'C') {
                                        Ready->push(OActive);
                                        OActive = NULL;
                                }
                                else if(OActive->history[OActive->sub]->type == 'N') {
                                        terminate(OActive, Timer);
					tProcesses ++;
                                        OActive = NULL;
                                }
                        }
                }
		else if(Output->length > 0) {// if there is no output process get one
			OActive = Output->pop();
			OActive->OCount ++;
			pObj* ptr = Output->first;
                        while(ptr != NULL) {
                                ptr->value->wait ++;
                                ptr = ptr->next;
                        }
		}
		pObj* ptrr = Ready->first;
		while(ptrr != NULL) {
			ptrr->value->wait ++;
			ptrr = ptrr->next;
		}
		//End of process management
		ptrr = Input->first;
		//check if a process needs to be added to the ready queue
		while (Ready->length < AT_ONCE && Entry->first != NULL && Entry->first->value->ArrivalTime == Timer) {
			cerr << "Process " << Entry->first->value->ProcessID << " moved from the Entry Queue into the Ready Queue at time " << Timer << "\n\n";
			Process* t = Entry->pop();
			Ready->push(t);
		}
		//if it is time, print the current status of the program
		if(Timer% HOW_OFTEN == 0) {
			//print time
			cerr << "\nStatus at time " << Timer << '\n';
			//print active process
			if(Active != NULL)
				cerr << "Active is " << Active->ProcessID << '\n';
			else
				cerr << "Active is 0\n";
			//print input process
			if(IActive != NULL)
				cerr << "IActive is " << IActive->ProcessID << '\n';
			else
				cerr << "IActive is 0\n";
			//print output process
			if(OActive != NULL)
				cerr << "OActive is " << OActive->ProcessID << '\n';
			else
				cerr << "OActve is 0\n";
			// print processes in the entry queue
			cerr << "Contents of the Entry Queue:\n";
			qObj* ptrE = Entry->first;
			while (ptrE != NULL) {
				cerr << ptrE->value->ProcessID << " Priotiry = " << ptrE->value->Priority << " ariving at time = " << ptrE->value->ArrivalTime << '\n';
				ptrE = ptrE->next;
			}
			//print processes in the Ready queue
			cerr << "Contents of the Ready Queue:\n";
			pObj* ptr = Ready->first;
			if(ptr == NULL){cerr << "(Empty)";}
			while (ptr != NULL) {
				cerr << ptr->value->ProcessID << '(' << ptr->value->Priority << ')' << "     ";
				ptr = ptr->next;
			}
			//Print processes in the Input queue
			cerr << "\nContents of the Input Queue:\n";
			ptr = Input->first;
			if(ptr == NULL){cerr << "(Empty)";}
			while (ptr != NULL) {
       	                	cerr << ptr->value->ProcessID << '(' << ptr->value->Priority << ')' << "     ";
       	                 	ptr = ptr->next;
       	         	}
			//Print processes in the Output queue
			cerr << "\nContents of the Output Queue:\n";
			ptr = Output->first;
			if(ptr == NULL){cerr << "(Empty)";}
			 while (ptr != NULL) {
                        	cerr << ptr->value->ProcessID << '(' <<  ptr->value->Priority << ')' << "     ";
                        	ptr = ptr->next;
                	}
			cerr << "\n\n";
		}
		//If there is nothing left to do, end the program
		if(Entry->length == 0 && Ready->length == 0 && Input->length == 0 && Output->length == 0 && Active == NULL && IActive == NULL && OActive == NULL) {
			cerr << "The run has ended.\n";
			//print final time
			cerr << "The final value of the timer was:  " << Timer << '\n';
			//print total idle time
			cerr << "The amount of time spent idle was: " << idleTime << '\n';
			//print total processes ran
			cerr << "Number of terminated processes =   " << tProcesses << '\n';
			//below is a copy of the status print
			if(Active != NULL)
                                cerr << "Active is " << Active->ProcessID << '\n';
                        else
                                cerr << "Active is 0\n";
                        if(IActive != NULL)
                                cerr << "IActive is " << IActive->ProcessID << '\n';
                        else
                                cerr << "IActive is 0\n";
                        if(OActive != NULL)
                                cerr << "OActive is " << OActive->ProcessID << '\n';
                        else
                                cerr << "OActive is 0\n";
                        cerr << "Contents of the Entry Queue:\n";
                        qObj* ptrE = Entry->first;
                        while (ptrE != NULL) {
                                cerr << ptrE->value->ProcessID << " Priotiry = " << ptrE->value->Priority << " ariving at time = " << ptrE->value->ArrivalTime << '\n';
                                ptrE = ptrE->next;
                        }
                        cerr << "Contents of the Ready Queue:\n";
                        pObj* ptr = Ready->first;
			if(ptr == NULL){cerr << "(Empty)";}
                        while (ptr != NULL) {
                                cerr << ptr->value->ProcessID << '(' << ptr->value->Priority << ')' << "     ";
                                ptr = ptr->next;
                        }
                        cerr << "\nContents of the Input Queue:\n";
                        ptr = Input->first;
			if(ptr == NULL){cerr << "(Empty)";}
                        while (ptr != NULL) {
                                cerr << ptr->value->ProcessID << '(' << ptr->value->Priority << ')' << "     ";
                                ptr = ptr->next;
                        }
                        cerr << "\nContents of the Output Queue:\n";
                        ptr = Output->first;
			if(ptr == NULL){cerr << "(Empty)";}
                         while (ptr != NULL) {
                                cerr << ptr->value->ProcessID << '(' <<  ptr->value->Priority << ')' << "     ";
                                ptr = ptr->next;
                        }
			break;//exit for loop
		}
	}
	return 0;//end program
}
