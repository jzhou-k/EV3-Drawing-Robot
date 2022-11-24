#pragma once
struct stringNode;
void stringNodeConstruct(struct stringNode &self);
bool stringNodeGetIndex(const stringNode &self, int index, string &out);
void stringNodeSetIndex(stringNode &self, int index, string newData);
void stringNodePop(stringNode &self, int index);
void stringNodeAppend(stringNode &self, string newData);
int stringNodeLength(const stringNode &self);

int stringNodeMaxAmount = 100;

struct stringNode
{
  string data;
  struct stringNode *next;
};
/*
struct stringNodeMem
{
	stringNode *allocatedMem[100];
}

/* gotta allocate our own memory as it seems as if you can not dynamicly create it*/
/*
void stringNodeMemConstruct(struct stringNodeMem &stringNodeMemory){
	stringNodeMemory.fillLevel = 0;
	for(int addr = 0; addr<stringNodeMaxAmount; addr++){
		stringNodeMemory.allocatedMem[addr] = NULL;
	}
}
*/

int getstringNodeMemFillLevel(struct stringNode *allocatedMem){
	int addr = 0;
	for(; addr<stringNodeMaxAmount; addr++)
	{
		if(allocatedMem[addr]==NULL){
			return addr;
		}
	}
	addr++;
	return addr;
}

/*
stringNode *stringNodeAllocateMem(struct stringNodeMem &stringNodeMemory){
	stringNode *tempPtr = stringNodeMemory.allocatedMem[stringNodeMemory.fillLevel];
	stringNodeMemory.fillLevel++;
	return tempPtr;
}

void stringNodeUnallocateMem(struct stringNode &stringNodeRemove){
	stringNode *tempPtr = stringNodeMemory.allocatedMem[stringNodeMemory.fillLevel];
	stringNodeMemory.fillLevel++;
	return tempPtr;
}
*/

void stringNodeConstruct(struct stringNode &self){
	self.data = "\0";
	self.next = NULL;
	return;
}

bool stringNodeGetIndex(const stringNode &self, int index, string &out){
	if(stringNodeLength(self)<=index){
		return false;
	}
	stringNode *tempPtr = &self;
	string data = "";
	for(int nodeDepth = 0; nodeDepth<index; nodeDepth++){
		tempPtr = (*tempPtr).next;
	}
	sprintf(out, (*tempPtr).data);
	return true;
}

void stringNodeSetIndex(stringNode &self, int index, string newData){
	stringNode *tempPtr = &self;
	for(int nodeDepth = 0; nodeDepth<index; nodeDepth++){
		if((*tempPtr).next == NULL){
			stringNode new;
			stringNodeConstruct(new);
			(*tempPtr).next = &new;
		}
		tempPtr = (*tempPtr).next;
	}
	(*tempPtr).data = newData;
	return;
}

void stringNodePop(stringNode &self, int index){
	return;
}

void stringNodeAppend(stringNode &self, string newData){
	return;
}

int stringNodeLength(const stringNode &self){
	stringNode *tempPtr = &self;
	int nodeDepth = 1;
	while(!((*tempPtr).next == NULL)) {
		nodeDepth++;
		tempPtr = (*tempPtr).next;
	}
	return nodeDepth;
}
