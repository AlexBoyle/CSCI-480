/**
*Alex Boyle
*Z1759628
*
*Assign1
*/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

//Main function
int main () {
	//Print current pid
	cerr << "\nOriginal: " + to_string(getpid()) + "\n";
	cerr << "\nCalling fork()\n";
	int inital_pid = getpid();
	int child = fork();
	// exitr if fork fails
	if(child < 0) {
		cerr << "The first fork failed.";
		return -1;
	}
	// If child
	if(child == 0){
		//print out list of pids
		cerr << "child(this): " + to_string(getpid()) + ", " + "parent: " + to_string(getppid()) + "\n";
		cerr << "\nCalling fork()\n";
		int grandchild = fork();
		if(grandchild < 0) {
			cerr << "The second fork failed.";
			return -1;
		}
		// If child
		if(grandchild == 0) {
			// print out list of pids
			cerr << "grandchild(this): " + to_string(getpid()) + ", " + "child: " + to_string(getppid()) + ", " + "parent: " + to_string(inital_pid) + "\n";
			// exit
			cerr << "\ngrandchild is about to exit\n";
			return 0;
		}
		else {
			//print out pids
			cerr << "child(this): " + to_string(getpid()) + ", " + "parent: " + to_string(getppid()) + "\n";
			// wait for children
			wait(0);
			// exit
			cerr << "\nchild is about to exit\n";
			return 0;
		}
	}
	else {
		// print pids
		cerr << "parent(this): " + to_string(getpid()) + ", grandparent: " + to_string(getppid()) + "\n";
		//wait 2 secs
		sleep(2);
		cerr << "about to call 'ps'\n";
		// call ps
		system("ps");
		// wait for children to exit
		wait(0);
		// exit
		cerr << "\nParent is about to exit\n";
		return 0;
	}
	cerr << '\n';
	return 0;
}
