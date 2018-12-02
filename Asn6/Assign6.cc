// Includes
#include <fstream>
#include <stddef.h>
#include <sstream> 
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <iostream>

// Constants
#define KB 1024
#define MB KB * KB

//Useful
using namespace std;

// Memory Block Class for keeping track of memory
class Block{
	public:
		Block(int size, int address) {
			this->size = size;
			this->startAddress = address;
		}
		int startAddress;
		int size;
		int pid;
		string name;
		Block * previous = NULL;
		Block * next = NULL;
};

// List of availble memory blocks
list<Block> available;
// List of in use memory blocks
list<Block> inUse;

// List of functions to be defined
void bRun();
void fRun();
void terminateProcess(string);
void insertToAvailable(Block);
void deallocate(string);
void bLoad(string);
void fLoad(string);
void print();

int startAddress = 3 * MB;

int main(int argc, char *argv[])
{
	char arguement = '?';
	if(argc > 1) {// Check for arguments
		arguement = argv[1][0];// Get argument
		if(!(arguement == 'B' || arguement == 'F')) {// Check for correct argument
			cerr << "\nuse the argument 'b' or 'f'\n";
			exit(-1);
		}
	} 
	else {
		cerr << "\nNo arguments givin\n";
		exit(-1);
	}
	int sizes[5] ={MB, 2 * MB, 2 * MB, 4 * MB, 4 * MB};// Inital memory sizes
	for(int i = 0; i < 5; i++){// Loop and initalize blocks
		available.push_back(*(new Block(sizes[i], startAddress)));
		startAddress = startAddress + sizes[i];
	}
	if(arguement == 'B') {// Start best fit 
		cerr<<"Simulation of Memory Management using the Best-Fit algorithm\n\nBeginning of the run\n\n";
		bRun();
	}
	else {// Start First Fit
		cerr<<"Starting Simulation with First-Fit method\n\nBeginning of the run\n\n";
		fRun();
	}
}
// Run the Best Fit memory system
void bRun() {
	ifstream file;
	file.open("data.txt");// Get File
	string line;
	char command = '?';
	print();// Print systems state
	while(!file.eof()) {// While there are lines to read
		getline(file,line);
		command = line[0];
		if(command == 'L')// Load process
			bLoad(line);
		else if(command == 'T')// Terminate process
			terminateProcess(line);
		else if(line[0] == 'A')// Allocate memory
			bLoad(line);
		else if(command == 'D')// Deallocate memory
			deallocate(line);
		else if(command == '?')// End
			break;
		print();// Print systems state
		
	}
	file.close();// Close file
	cerr<<"Simulation Ended\n\n";
	print();// Print systems state
}

// Run the First Fit memory system
void fRun() {
	ifstream file;
	file.open("data.txt");// Get File
	string line;
	char command = '?';
	print();// Print systems state
	while(!file.eof()) {
		getline(file,line);
		command = line[0];
		if(command == 'L')// Load process
			fLoad(line);
		else if(command == 'T')// Terminate process
			terminateProcess(line);
		else if(line[0] == 'A')// Allocate memory
			fLoad(line);
		else if(command == 'D')// Deallocate memory
			deallocate(line);
		else if(command == '?')// End
			break;
		print();// Print systems state
		
	}
	file.close();// close file
	cerr<<"Simulation Ended\n\n";
	print();// Print systems state
}
// Deallocate a memory block
void deallocate(string line) {
	char command;
	int id;
	string name;
	istringstream str(line);// Use line as input
	str >> command >> id >>name;// Break apart input line
	cerr << "Transaction:  request to deallocate block ID " << name << " for process " << id << '\n';
	list<Block>::iterator it1;
	for(it1 = inUse.begin(); it1 != inUse.end(); it1++){// Loop through inUse blocks
		if(it1->pid == id && it1->name == name){// Look for block to deallocate
			insertToAvailable(*(new Block(it1->size, it1->startAddress)));// Put block into available
			inUse.erase(it1);// Remove block from inUse
			cerr<<"Success in deallocating a block\n\n";
			return;
		}
	}
	cerr<<"Unable to comply as the indicated block cannot be found.\n\n";
}
// Terminate a process
void terminateProcess(string line){
	char command;
	int id;
	istringstream str(line);// Use line as input
	str >> command >> id;// Break apart input line
	list<Block>::iterator it1;
	cerr << "Transaction:  request to terminate process " << id << '\n';
	bool isTerminated = false;
	for(it1 = inUse.begin(); it1 != inUse.end(); it1 ++){// Loop through inUse
		if(it1->pid == id){// If a block has the pid of the terminated process
			insertToAvailable(*(new Block(it1->size, it1->startAddress)));// Put block in available
			inUse.erase(it1);// Remove from inUse
			it1--;
			isTerminated = true;
		}
	}
	if(!isTerminated)// Check if any blocks were removed
		cerr<<"Unable to comply as the indicated process could not be found.\n\n";
	else
		cerr<<"Success in terminating a process\n\n";
}
// Allocate Memory as First Fit
void fLoad(string line){
	char command;
	int id;
	int size;
	string name;
	bool isavailble = false;
	istringstream str(line);// Use line as input
	str >> command >> id >> size >>name;// Break apart input line
	list<Block>::iterator it1;
	if(command == 'L')// Check what we are doing
		cerr << "Transaction:  request to load process " << id << ", block ID " << name << " using " << size << " bytes\n";
	else
		cerr << "Transaction:  request to allocate " << size << " bytes for process " << id << ", block ID: " << name << "\n";
	for(it1 = available.begin(); it1 != available.end(); ++it1){// Look for the first memory block we can use
		if((it1->size - size) >= 0){
			isavailble = true;
			cerr << "Found a block of size " << it1->size << '\n';
			break;
		}
	}
	if(isavailble){// If there is a block we can use, start using it
		it1->size = it1->size - size;
		Block mem(size, it1->startAddress);
		it1->startAddress = it1->startAddress + size;
		mem.pid = id;
		mem.name = name;
		inUse.push_front(mem);
		
		cerr << "Success in allocating a block\n\n";
	}
	else {
		cerr << "Unable to comply as no block of adequate size is available\n\n";
	}
	
}
// Allocate Memory as Best Fit
void bLoad(string line){
	char command;
	int id;
	int size;
	string name;
	bool isavailble = false;
	istringstream str(line);// Use line as input
	str >> command >> id >> size >>name;// Break apart input line
	list<Block>::iterator it1;
	list<Block>::iterator it2;
	int memoryAvl = 15 * MB;
	if(command == 'L')
		cerr << "Transaction:  request to load process " << id << ", block ID " << name << " using " << size << " bytes\n";
	else 
		cerr << "Transaction:  request to allocate " << size << " bytes for process " << id << ", block ID: " << name << "\n";
	for(it1=available.begin(); it1 != available.end(); ++it1){// Find the best fitting memory block
		if((it1->size - size) < memoryAvl && (it1->size - size) > 0){
			memoryAvl = (it1->size - size);
			it2 = it1;
			isavailble = true;
		}
	}
	if(isavailble){// if there is one, use it
		cerr << "Found a block of size " << it2->size << '\n';
		it2->size = it2->size - size;
		Block mem(size, it2->startAddress);
		it2->startAddress = it2->startAddress + size;
		mem.pid = id;
		mem.name = name;
		inUse.push_back(mem);
		cerr << "Success in allocating a block\n\n";
	}
	else{
		cerr << "Unable to comply as no block of adequate size is available\n\n";
	}
	
}
// Move blocks to availble/Merge blocks
void insertToAvailable(Block mem){
	list<Block>::iterator it1;
	list<Block>::iterator it2;
	bool isInAvailable = false;
	for(it1 = available.begin(); it1 != available.end(); it1++) {// Look for the block in available
		if(it1->startAddress > mem.startAddress){
			available.insert(it1,mem);
			isInAvailable = true;
			break;
		}
	}
	if(!isInAvailable) {// if not in available, put it there
		available.push_front(mem);
	}
	for(it1=available.begin(); it1 != --available.end(); it1++) {// Look for blocks to merge
		it2 = it1;
		it2++;
		if((it1->size + it1->startAddress) == (it2->startAddress)) {// if two available blocks are next to eachother, MERGE
			if((it1->size + it2->size) <= 4*MB) {
				cerr << "Merging two blocks at " << it1->startAddress << " and " << it2->startAddress << '\n';
				it1->size = it1->size + it2->size;
				available.erase(it2);
				it1--;
			}
		}
	}
}
// Print the current status of the system
void print(){
	list<Block>::iterator it1;
	cerr << "List of available blocks\n";
	int total = 0;
	if(available.size() > 0) {
		for(it1=available.begin(); it1 != available.end(); ++it1){// Loop and print availble memory
			cerr<<"Start Address =" << setw(9) << it1->startAddress;
			cerr<<" Size =" << setw(9) << it1->size << '\n';
			total = total + it1->size;
		}
	}
	else {
		cerr<<"(none)\n";
	}
	cerr<<"Total size of the list is: "<<total<< "\n\n";
	cerr<<"List of blocks in use\n";
	total = 0;
	if(inUse.size() > 0) {
		for(it1=inUse.begin(); it1 != inUse.end(); ++it1){// Loop and print inUse memory
			cerr<<"Start Address =" << setw(9) << it1->startAddress;
			cerr<<" Size =" << setw(8) << it1->size;
			cerr<<" Process ID =" << setw(5) << it1->pid;
			cerr<<" Block ID =" << setw(6) << it1->name << '\n';
			total = total + it1->size;
		}
	}
	else {
		cerr<<"(none)\n";
	}
	cerr<<"Total size of the list is: "<<total<< "\n\n";
}
