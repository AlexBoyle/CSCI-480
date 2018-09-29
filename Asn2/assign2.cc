/**
*Alex Boyle
*Z1759628
*
*Assign2
*/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int pipeA[2];
int pipeB[2];
int pipeC[2];

// Parent Body
int PWork() {
	// Close pipes
	close(pipeA[0]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[1]);

	// Set read and write IDs
	int readID = pipeC[0];
	int writeID = pipeA[1];
	int  m = 1;
	char WriteBuff[100];
	cerr << "Created Parent\n";

	// Wait for every prosses to be ready
	sleep(1);

	// Send inital value
	cerr << "\nParent:     1\n";
	write(writeID, "1", 2);

	// While m is too small
	while(m < 99999999) {
		char buff[10] = "";
		char* buffChar;

		// Read input one byte at a time; store in buff
		do {
			read(readID, buffChar, 1);
			strcat(buff, buffChar);
		} while(buffChar[0] != '\0');

		// Convert buff to num
		m = atoi(buff);

		// Calculate new number
		m = (4 * m) + 3;
		sprintf(buff, "%d", m);
		sprintf(WriteBuff, "Parent:     %d\n",m);
		cerr << WriteBuff;

		// Send new number to next prossess
		write(writeID, buff, strlen(buff) + 1);
	}

	return 1;
}

// Child Body
int CWork() {
	// Close pipes
	close(pipeB[0]);
        close(pipeC[0]);
        close(pipeC[1]);
        close(pipeA[1]);

	// Set read and write IDs
        int readID = pipeA[0];
        int writeID = pipeB[1];

	int m = 1;
	char WriteBuff[100];
	cerr << "Created Child\n";

	// While m is too small
        while(m < 99999999) {
                char buff[10] = "";
                char* buffChar;

		// Read from the pipe one byte at a time; store in buff
		do {
                        read(readID, buffChar, 1);
			strcat(buff, buffChar);
                } while(buffChar[0] != '\0');

		// Convert anc calculate new m
                m = atoi(buff);
                m = 4 * m + 3;
                sprintf(buff, "%d", m);
		sprintf(WriteBuff, "child:      %d\n",m);
		cerr << WriteBuff;

		// Write to next prossess
		write(writeID, buff, strlen(buff) + 1);
	}

	return 1;
}

// Grandchild Body
int GWork() {
	// Close Pipes
	close(pipeC[0]);
        close(pipeA[0]);
        close(pipeA[1]);
        close(pipeB[1]);

	// Set read and write IDs
        int readID = pipeB[0];
        int writeID = pipeC[1];

	int m = 1;
	char WriteBuff[1000];
	cerr << "created Grandchild\n";

	// While m is too small
	while(m < 99999999) {
                char buff[10]  = "";
		char* buffChar;

		// Read from pipe one byte at a time; save in buff
               do{
			read(readID, buffChar, 1);
		        strcat(buff, buffChar);
                }while(buffChar[0] != '\0');

		// Convert and calculate
                m = atoi(buff);
                m = 4 * m + 3;

                sprintf(buff, "%d", m);
		sprintf(WriteBuff, "Grandchild: %s\n",buff);
                cerr << WriteBuff;

		// Write to next prossess
                write(writeID, buff, strlen(buff) + 1);
        }
	return 1;
}

//Main function
int main () {
	// Open all pipes
	if(pipe(pipeA) == -1) {cerr << "Failed to create Pipe A"; return -5;}
	if(pipe(pipeB) == -1) {cerr << "Failed to create Pipe B"; return -5;}
	if(pipe(pipeC) == -1) {cerr << "Failed to create Pipe C"; return -5;}

	int pid = fork();
	if(pid == -1) {cerr << "Failed to Fork child";}
	// start Parent prossess
	else if(pid != 0) { return PWork();}
	else {
		pid = fork();
		if(pid == -1) {cerr << "Failed to Fork grandchild";}
		// Start child prossess
		else if(pid != 0) {return CWork();}
		// Start grandchild prossess
		else { return GWork();}
	}
	return 0;

}
