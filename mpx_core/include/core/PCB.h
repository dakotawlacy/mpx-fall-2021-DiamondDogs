#ifndef _PCB_H
#define _PCB_H


struct PCB AllocatePCB();

struct PCB FindPCB(char* name);

struct PCB FreePCB(PCB* pcbName);

struct PCB SetupPCB(char* processName, int class, int priority);

struct PCB InsertPCB(PCB* pcbName);

struct PCB RemovePCB(PCB* pcbName);

#endif
