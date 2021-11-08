#ifndef _MCB_H
#define _MCB_H

#include <core/struct.h>

extern list heapList;

void initHeap();

void initLists();

MCB* allocateMem(int size);

void freeMem(u32int location);

void updateList();

struct MCB* findSpace(int size);

int isEmpty();

void showFree();

void showAlloc();

void printNodes();

#endif
