#ifndef _MCB_H
#define _MCB_H

#include <core/struct.h>

extern list heapList;


void initHeap();

void initLists();

void allocateMem();

struct MCB* findSpace(int size);

void printNodes();

#endif
