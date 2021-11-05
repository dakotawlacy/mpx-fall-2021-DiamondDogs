#ifndef _MCB_H
#define _MCB_H

#include <core/struct.h>

extern list freeList;
extern list allocList;

void initHeap();

void initLists();

void insertMCB(MCB* mcb);
//
void insertFree(MCB* mcb);

//void insertAlloc(MCB* mcb);

#endif
