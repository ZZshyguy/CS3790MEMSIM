#include "memory.h"

/*
Zachary Lapointe
CS2010
Memory simulator using a class (linked lists)
*/

int main(void) {
	//srand(666);
	srand (time(NULL));
	long clock = 0;
	int lease, size;
	int nummerge = 0;
	int numreq = 0;
	bool goodreq=true;
	int numbad = 0;
	memory sim;

   	while(clock < TIME_LIMIT){
		if(clock%REQUEST_INTERVAL == 0){
        		/* generate a memory request */
			if(numreq != 0) sim.memsort();
			lease = rand() % (MAX_LEASE - MIN_LEASE) + MIN_LEASE;
			size = rand() % (MAX_SIZE - MIN_SIZE) + MIN_SIZE;
			numreq++;
			goodreq = true;
			if(!sim.requestmem(size,lease,clock)){
				sim.memmerge();
				nummerge++;
				if(!sim.requestmem(size,lease,clock)) goodreq = false;
			}
			if(goodreq == false)numbad++;
		}
	/* see if lease has expired  */
	sim.checklease(clock);
	clock++;
   	}
	cout<< "Number of requests: " << numreq << endl;
	cout<< "Number of satisfied requests: " << numreq-numbad << endl;
	cout<< "Number of failed requests: " << numbad << endl;
	cout<< "Number of merges: " << nummerge << endl;
	sim.dump();
}


