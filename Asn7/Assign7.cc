// Includes
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> 
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <list>
#include <iomanip>
 
using namespace std;
// Doc said 6, the example output is 5
#define HOWOFTEN	5
class  File{
	public:
	int size;
	string name;
	vector<int> blocks;
};
//Initialzie vars
int blockNum = 1;
void newFile(string line, bool isCommand);
void deleteFile(string line, bool isCommand);
void copyFile(string line, bool isCommand);
void renameFile(string line, bool isCommand);
void modifyFile(string line, bool isCommand);
// Print empty system
void print();
//Directory Vector to hold the blocks of memory in order
vector<int> directory(4096);
//List of files
vector<File> files;
int blockSize = 512;

/**
*Main function
*
*
*/
int main(int argc, char *argv[]) {
	//create blocks
	for(int i=0; i < directory.size(); i++) {
		directory[i] = 0;
	}
	ifstream file;
	file.open("data.txt");
	string line;

	//counters
	int j=0;
	int t = 0;
	cerr << "Beginning of the FAT simulation\n\n";

	//first two elements 
	newFile("N . 512", false);
	newFile("N .. 0", false);

	//go through file and read line by line
	while(!file.eof()) {
		if(t%HOWOFTEN == 0)
			print();// every HOWOFTEN

		getline(file,line);
		// Rename File
		if(line.at(0) == 'R') {
			cerr<< "Transaction:  Rename a file\n";
			renameFile(line, true);
		}
		// Modify File
		else if(line.at(0) == 'M') {
			cerr<< "Transaction:  Modify a file\n";
			modifyFile(line, true);
		}
		// New File
		else if(line.at(0) == 'N') {
			cerr<< "Transaction:  Add a new file\n";
			newFile(line, true);
		}
		// Delete File
		else if(line.at(0) == 'D') {
			cerr<< "Transaction:  Delete a file\n";
			deleteFile(line, true);
		}
		// Copy File
		else if(line.at(0) == 'C') {
			cerr<< "Transaction:  Copy a file\n";
			copyFile(line, true);
		}
		// End Program
		else if(line.at(0) == '?')
			break;

		t++;
	}
	file.close();
	cerr << "\nEnd of the FAT simulation\n";
	print();

}
/**
*Create a file
*
*Takes an input line and a command bool
*/
void newFile(string line, bool isCommand) {
	char trans;
	string name;
	int size;
	bool found = false;
	 File file;
	//get info from line
	istringstream iss(line);
	iss >> trans >> name >> size;

	//check for file existence 
	for(int i=0; i < files.size(); i++){
		if(files.at(i).name == name) {
			cerr<<"File already exists can not create new file: "<<name<< "\n";
			return;
		}
	}
	file.name = name;
	file.size = size;
	int amountOfBlocks = size / blockSize;
	if(size % blockSize > 0 )
		amountOfBlocks++;
	for(int i = 1;i <= amountOfBlocks;){
		for(int j=0; j < directory.size(); j++){
			if(directory.at(j) == 0 && i == amountOfBlocks){
				directory.at(j) = -1;
				blockNum ++;
				file.blocks.push_back(j);
				i++;
				break;
			}
			else if(directory.at(j) == 0){
				directory.at(j) = blockNum;
				blockNum ++;
				file.blocks.push_back(j);
				i++;
				break;
			}
		}
	}
	//put object in list
	if(isCommand)
		cerr << "Successfuly added a new file, " << name<<", of size " << size<<'\n';
	files.push_back(file);
	
}

/**
*Delete a file
*
*Takes an input line and a command bool
*/
void deleteFile(string line, bool isCommand){

	char trans;
	string name;
	istringstream iss(line);
	iss >> trans >> name;

	for(int i = 0; i < files.size(); i++){
		if(name == files.at(i).name){
			for(int j = 0; j < files.at(i).blocks.size(); j++)
				directory.at(files.at(i).blocks.at(j)) = 0;
			files.erase(files.begin() + i );//erase file from list
		}
	}
	if(isCommand)
		cerr << "Successfully deleted a file, " + name << '\n';
}

/**
*Copy a file
*
*Takes an input line and a command bool
*/
void copyFile(string line, bool isCommand) {
	char trans;
	string name;
	string newName;
	bool found = false;
	int size = 0;
	istringstream iss(line);
	iss >> trans >> name >> newName;
	for(int i = 0; i < files.size(); i++){
		if(files.at(i).name == name){
			size = files.at(i).size;
			found = true;
		}
	}
	for(int j = 0; j < files.size(); j++){
		if(files.at(j).name == newName){
			cerr<<"Copy with the same name as the new copy found: "<<newName<< "\n";
			return;
		}
	}
	if(!found){
		cerr<<"Cannot find file to copy: "<<name<< "\n";
	}
	else{
		newFile("N " + newName + " " + to_string(size), false);
		if(isCommand)
			cerr << "Successfully copied an existing file, " + name + ", to a new file, " + newName << '\n';
	}
}
/**
*Rename File
*
*Takes an input line and a command bool
*/
void renameFile(string line, bool isCommand) {
	char trans;
	string name;
	string newName;
	bool found = false;
	int index;
	istringstream iss(line);
	iss >> trans >> name >> newName;
	for(int i = 0; i < files.size(); i++){
		if(files.at(i).name == name){
			found = true;
			index = i;
		}
		if(files.at(i).name == newName) {
			cerr<<"File with name of copy already exists: "<<newName<< "\n";
			return;
		}
	}
	if(!found){cerr<<"No File found to be renamed, File: "<<name<< "\n";}
	else{files.at(index).name = newName;if(isCommand){cerr<<"Successfully changed the file name " + name + " to " + newName << '\n';}}
}
/**
*Modify a file
*
*Takes an input line and a command bool
*/
void modifyFile(string line, bool isCommand){
	char trans;
	string name;
	string num;
	istringstream iss(line);
	iss >> trans >> name >> num;
	newFile("N " + name + "(copy) " + num, false);
	deleteFile("D " + name, false);
	renameFile("R " + name + "(copy) " + name, false);
	if(isCommand)
		cerr << "Successfully modified a file, " + name + "\n";
}

/**
*Print current system
*/
void print(){
	int j=0;
	int totalSize = 0;
	cerr << "\nDirectory Listing\n";
	for(int i =0; i < files.size(); i++) {
		cerr<<"File Name: " << setw(20) << left <<files.at(i).name;
		cerr <<"    File Size: "<<files.at(i).size<< "\n";
		totalSize += files.at(i).size;
		cerr<<"Cluster(s) in use:";
		for(int j=0; j < files.at(i).blocks.size(); j++) {
			if(j%12==0&&j!=0){cerr<<"\n                  ";}
			cerr<< setw(6) << right <<files.at(i).blocks.at(j);
		}
		if( files.at(i).blocks.size() == 0) {
			cerr << "(none)";
		}
		cerr<< "\n\n";
	}
	cerr<<"Files: "<<files.size()<<"\t"<<"Total Size: "<<totalSize<< "\n\n";
	for(int i =0; i < 240; i++){
		if(j % 12 == 0){
			cerr<< "#" <<setw(3)<<setfill('0') << i <<" - "<<setw(3)<<setfill('0')<<i+11<<": "<<"\t"<< setfill(' ');
		}
		cerr<<right<<setw(2)<<directory.at(i)<<"\t";
		j++;
		if(j % 12 == 0){
			cerr<< "\n";
			j=0;
		}
	}
	cerr<< "\n";
}