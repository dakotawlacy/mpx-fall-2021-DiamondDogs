#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/struct.h>
#include <core/PCB.h>

struct PCB* AllocatePCB(){
  sys_alloc_mem(1084);
  return AllocatePCB;
}

struct PCB* FindPCB(char* name){
  if(strcmp(&FindPCB->process_name, name) == 0){
    return FindPCB;
  }
    return NULL;
}

struct PCB* FreePCB(struct PCB* pcbName){
  sys_free_mem(pcbName);
  return pcbName;
}

struct PCB* SetupPCB(char* processName, int class, int priority){
  AllocatePCB();
  SetupPCB->process_name = processName;
  SetupPCB->process_class = class;
  SetupPCB->priority = priority;
  return SetupPCB;
}

struct PCB* InsertPCB(struct PCB* pcbName){
  //Ready Or Blocked
  pcbName->next = InsertPCB;
  if(strcmp(pcbName->processName, pcbName)!= 0){
    return NULL;
  }
  return InsertPCB;
}

struct PCB* RemovePCB(struct PCB* pcbName){
  //Ready Or Blocked
  RemovePCB->previous = pcbName;
  return pcbName;
}
