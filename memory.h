#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#define MIN_LEASE 40
#define MAX_LEASE 70
#define MIN_SIZE 50
#define MAX_SIZE 350
#define TIME_LIMIT 1000
#define MEMORY_SIZE 1000
#define REQUEST_INTERVAL 10


using namespace std;


struct range {
	int start;		// the starting address of the range
	int size;		// size of the range
};


struct freeNode {
	struct range hole;
	struct freeNode *next;	// pointer to next node
};


struct allocNode {
	struct range allocated;
	int leaseExpiry; 	// time at which this block will be returned to free list
	struct allocNode *next;	//pointer to next node;
};


class memory {
private:
	freeNode *freelist;
	allocNode *alloclist;
	int size;
public:
	memory();
	void checklease(int clock);
	bool memmerge();
	void memsort();
	bool requestmem(int size, int lease, int clock);
	void dump();
};


#endif



