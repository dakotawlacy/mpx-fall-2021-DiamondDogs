#ifndef _PCB_H
#define _PCB_H

#include <core/struct.h>

extern queue readyQueue;
extern queue blockedQueue;
extern queue suspendedReady;
extern queue suspendedBlock;

void get_pcb_data(char* commandBuff);

struct PCB* allocatePCB();

int freePCB(PCB* pcbName);

struct PCB* setupPCB(char* processName, int class, int priority);

struct PCB* insertPCB(PCB* pcbName);

int removePCB(PCB* pcb);

void printReady();
void deletePCB(char* commandBuff);
void showPCB(char* commandBuff);

void initQueues();
PCB* findPCB(char* name);

#endif
