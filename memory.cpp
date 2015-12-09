#include "memory.h"

memory::memory(){
//Memory constructor, MEMORY_SIZE is 1000
	size = MEMORY_SIZE;
	alloclist = new allocNode;
	freelist = new freeNode;
	freelist->hole.start = 0;
	freelist->hole.size = MEMORY_SIZE;
}


void memory::checklease(int clock){
//Check allocList for expired leases, and move them to freelist
	allocNode *tmp = this->alloclist;
	freeNode *end = this->freelist;
	while(end != NULL){ //Make a pointer to the end of freelist
		end = end->next;
	}
	while(tmp != NULL){
		if(tmp->leaseExpiry == clock){
			freeNode * tmp2 = new freeNode;//Make new freeNode
			tmp2->hole.start = tmp->allocated.start;
			tmp2->hole.size = tmp->allocated.size;
			end->next = tmp2; //Make end of freelist point to new node
			end = tmp2; //Move end point over
			delete tmp2;

			allocNode *del = tmp->next;
			tmp->next = tmp->next->next;
			delete del; 
			//Adjust pointer to point to next freeNode in list(or null)
			//Basically deletes the node that was merged
		}
		tmp = tmp->next;
	}
	
	delete end;
	delete tmp;
}


bool memory::memmerge(){
//Checks freelist nodes, and merges the holes if able
	bool good = false;
	freeNode *tmp = this->freelist;
	while(tmp != NULL){
		if(tmp->hole.size == tmp->next->hole.start){
			tmp->hole.size = tmp->hole.size + tmp->next->hole.size;
			//Adjust the size
			freeNode *del = tmp->next;
			tmp->next = tmp->next->next;
			delete del;
			//Adjust pointer to point to next freeNode in list(or null)
			//Basically deletes the node that was merged
			good = true; //Bool to check if a merge happened
		}
		tmp = tmp->next;
	}
	
	delete tmp;

	return good;
}


void memory::memsort(){
//Sorts both lists
	//Sorting a linked list
	//FREELIST
	freeNode * temphead = this->freelist;
	freeNode * tempnode = NULL;
	int counter = 0;
	while (temphead)
	{
		temphead = temphead->next;
		counter++;
	}
	temphead = this->freelist;
	
	for (int j=0; j<counter; j++)
	{
		while (temphead->next)  //iterate through list until next is null
		{
			if (temphead->hole.start > temphead->next->hole.start)
			{
				tempnode = temphead;
				temphead = temphead->next;
				tempnode->next = temphead->next;
				temphead->next = tempnode;
				
				temphead = temphead->next;//increment node
			}
			else 
				temphead = temphead->next;//increment node
		}	
		temphead = this->freelist;//reset temphead
	}
	delete temphead;
	delete tempnode;

	//ALLOCATED LIST
	allocNode *temphead2 = this->alloclist;
	allocNode *tempnode2 = NULL;
	counter = 0;
	while (temphead2)
	{
		temphead2 = temphead2->next;
		counter++;
	}
	temphead2 = this->alloclist;
	
	for (int j=0; j<counter; j++)
	{
		while (temphead2->next)  //iterate through list until next is null
		{
			if (temphead2->leaseExpiry > temphead2->next->leaseExpiry)
			{
				tempnode2 = temphead2;
				temphead2 = temphead2->next;
				temphead2->next = tempnode2;
				
				temphead2 = temphead2->next;//increment node
			}
			else 
				temphead2 = temphead2->next;//increment node
		}	
		temphead2 = this->alloclist;//reset temphead
	}
	delete temphead2;
	delete tempnode2;
}


bool memory::requestmem(int size, int lease, int clock){
//Allocate a new allocNode of a certain size
	bool good = false;
	freeNode *finder = this->freelist;
	allocNode *end = this->alloclist;
	while(end != NULL){ //Make a pointer to the end of alloclist
		end = end->next;
	}
	while(finder != NULL){
		if(size <= finder->hole.size){
			allocNode *tmp = new allocNode;
			//Assign correct values based off freelist
			tmp->leaseExpiry = lease+clock; 
			tmp->allocated.start = finder->hole.start;
			tmp->allocated.size = size;
			end->next = tmp; //Make end of alloclist point to new node
			end = end->next;
			delete tmp;
			
			//Change freeNode values
			finder->hole.start = finder->hole.size - size;
			finder->hole.size = finder->hole.size - finder->hole.start;
			
			good = true;
			break;
		}
	}
	return good;	
}


void memory::dump(){
//Dump various statistics and the current state of free/alloc lists
	freeNode *tmpf = this->freelist;
	allocNode *tmpa = this->alloclist;

	cout<< "********ALLOCATED LIST********" << endl;
	while(tmpa != NULL){
		cout << "Lease: " << tmpa->leaseExpiry << "   Start: " << tmpa->allocated.start
			<<"   Size: "<< tmpa->allocated.size << endl;
		tmpa = tmpa->next;
	}

	cout<< "**********FREE LIST**********" << endl;
	while(tmpf != NULL){
		cout << "   Start: " << tmpf->hole.start
			<<"   Size: "<< tmpf->hole.size << endl;
		tmpf = tmpf->next;
	}

	delete tmpf;
	delete tmpa;
}









