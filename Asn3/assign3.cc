#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>
using namespace std;
/**
*Child
* params: char* const* argv - params for running a prossess
*
*returns nothing, prosses that is started should take over thread
*
*/
void child(char* const* argv) {
	execvp(argv[0], argv);
        printf("couldn't execute: %s\n", argv[0]);
       	exit(127);
}

int main(void) {
	int argNum = 32; // Max number of arguments is 32, probs safe
	char buf[1024];// Input buffer
	int pid; // Prosses id
	int status; // Command Status
	printf("480shell> "); // Print terminal header
	while (fgets(buf,1024,stdin) != NULL) {
		buf[strlen(buf) -1] = '\0';  //remove the last character. Important!
		char* argv[argNum]; // array to hold individual args
		argv[0] = strtok(buf, " ");// There should be atleast one arg
		int lastIndex = 0;// Keep track of index
		for(int i = 1; i < argNum; i ++) {// Loop thorugh  args to create argv array
			argv[i] = strtok(NULL, " ");
			if(argv[i] == NULL) {
				lastIndex = i - 1;
				argv[i] = NULL;// Set the last index to null
				break;
			}
		}
		// Check if we should quit
		if(strcmp(argv[0], "q") == 0 || strcmp(argv[0], "quit") == 0) {exit(1);}
		// Check for pipe
		int indexOfPipe = -1;
		for(int i = 1; i < argNum; i ++){
			if(argv[i] != NULL) {
				if(strcmp(argv[i], "||") == 0) {
					indexOfPipe = i;
					argv[i] = NULL;
				}
			}
			else {break;}
		}
		// Pipes to be piped
		int pipeA[2];
		int pipeB[2];
		if(indexOfPipe > 0) {// If there is a pipe
			pipe(pipeA);// we need two pipes
			pipe(pipeB);
			if ((pid = fork()) < 0)// fork off of parent
				printf("fork error");
			else if (pid == 0) {
				close(1);// close unneeded pipes
				close(pipeB[0]);
				close(pipeA[0]);
				close(pipeA[1]);
				dup2(pipeB[1],1);// Dup write pipe
				child(argv);//Run first command
			}
			if ((pid = fork()) < 0)// Fork off of parent
                                printf("fork error");
                        else if (pid == 0) {
				close(1);//close unneeded pipes
				close(0);
                                close(pipeA[0]);
                                close(pipeB[1]);
				dup2(pipeB[0],0);// Dup read pipe
				dup2(pipeA[1],1);// Dup write pipe
                                child(&argv[indexOfPipe+1]);// Run Second Command
			}
			close(pipeB[0]);// Close parent pipes
			close(pipeB[1]);
			close(pipeA[1]);
		}
		else {
			pipe(pipeA);
			if ((pid = fork()) < 0)
				printf("fork error");
			else if (pid == 0) { /* child */
				close(1);// Close pipes
				close(pipeA[0]);
				dup2(pipeA[1],1);// Dup write pipe
				child(argv);
			}
			close(pipeA[1]);// close parent pipe
		}
		/* parent */
		if ( (pid = waitpid(pid, &status, 0)) < 0)
			printf("waitpid error");
		char buff[2];
		buff[1] = '\0';
		while(read(pipeA[0], buff, 1) != 0){// Print one char at a time
			cerr << buff;
		}
		printf("\n480shell> ");
	}
	exit(0);
}
