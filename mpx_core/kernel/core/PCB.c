#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/struct.h>
#include <core/PCB.h>

struct PCB* AllocatePCB(){
  struct PCB* Pcb;
  sys_alloc_mem(1084);
  return Pcb;
}

struct PCB* FindPCB(char* name){
  struct PCB* Pcb;
  if(strcmp(Pcb->process_name, name) == 0){
    return Pcb;
  }
    return NULL;
}

struct PCB* FreePCB(struct PCB* pcbName){
  sys_free_mem(pcbName);
  return pcbName;
}

struct PCB* SetupPCB(char* processName, int class, int priority){
  struct PCB* pcb;
  AllocatePCB();
  pcb->process_name = processName;
  pcb->process_class = class;
  pcb->priority = priority;
  return pcb;
}

struct PCB* InsertPCB(struct PCB* pcbName){
  //Ready Or Blocked
  struct PCB* pcb;
  pcbName->next = pcb;
  if(strcmp(pcbName->process_name, pcb->process_name)!= 0){
    return NULL;
  }
  return pcb;
}

struct PCB* RemovePCB(struct PCB* pcbName){
  //Ready Or Blocked
  struct PCB* pcb;
  pcb->previous = pcbName;
  return pcbName;
}
