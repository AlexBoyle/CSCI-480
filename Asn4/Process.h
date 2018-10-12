#ifndef PROCESS
#define PROCESS
/**
*A structure that will be used to hold history information
*
*/
struct histObj {
	char type = NULL;
	int value = NULL;
	histObj(char _type, int _value){type = _type; value = _value;}
	histObj(){}
};
/**
*Contains all information a process needs
*/
class Process {
        public:
		Process();
                Process(char*, char*, char*);
		char* ProcessName = NULL;
		int Priority = -1;
		int ProcessID = -1;
		int ArrivalTime = -1;
		histObj *history[10];
		int sub = 0;
		int CPUTimer = 0;
		int IOTimer = 0;
		int CPUTotal = 0;
		int ITotal = 0;
		int OTotal = 0;
		int CPUCount = 0;
		int ICount = 0;
		int OCount = 0;
		int wait = 0;
};
#endif

