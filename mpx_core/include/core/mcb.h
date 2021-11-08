#ifndef _MCB_H
#define _MCB_H

#include <core/struct.h>

extern list heapList;

void initHeap();

void initLists();

u32int allocateMem(u32int size);

int freeMem(void* location);

void updateList();

struct MCB* findSpace(int size);

int isEmpty();

void showFree();

void showAlloc();

void printNodes();

#endif
