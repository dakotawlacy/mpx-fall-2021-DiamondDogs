#ifndef _PCB_H
#define _PCB_H

#include <core/struct.h>

extern queue readyQueue;
extern queue blockedQueue;
extern queue suspendedReady;
extern queue suspendedBlock;

extern PCB* cop;

char* get_name(char* commandBuff);

char* get_class(char* commandBuff);

char* get_prio(char* commandBuff);

struct PCB* allocatePCB();

void freePCB(PCB* pcbName);

int get_pcb_data(char* commandBuff);

struct PCB* setupPCB(char* processName, int class, int priority);

struct PCB* insertPCB(PCB* pcbName);

int removePCB(PCB* pcb);

void printReady();

void printBlock();

void print(PCB* q);

void printAll();

void deletePCB(char* commandBuff);

int showPCB(char* commandBuff);

int setPriority(char* commandBuff);

struct PCB* findPCB(char* name);

struct PCB* blockPCB(char* commandBuff);

struct PCB* unblockPCB(char* commandBuff);

struct PCB* suspendPCB(char* commandBuff);

struct PCB* resumePCB(char* commandBuff);

void initQueues();

#endif
