#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/PCB.h>
#include <core/testProcesses.h>
#include <core/loadr3.h>

int checkForRun = 0;
int i;

void loadr3(){

  if(checkForRun != 0){
    char* loadr3_error = "Loadr3 has already ran\n";
    int lr3_err = strlen(loadr3_error);
    sys_req(WRITE,DEFAULT_DEVICE, loadr3_error, &lr3_err);
    return;
  }

  context* cp;
  PCB* newPCB;


  newPCB = setupPCB("proc1",1,1);
  cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &proc1;
  cp->eflags = 0x202;

  newPCB = setupPCB("proc2",1,2);
  cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &proc2;
  cp->eflags = 0x202;

  newPCB = setupPCB("proc3",1,3);
  cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &proc3;
  cp->eflags = 0x202;

  newPCB = setupPCB("proc4",1,4);
  cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &proc4;
  cp->eflags = 0x202;

  newPCB = setupPCB("proc5",1,5);
  cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &proc5;
  cp->eflags = 0x202;

  return;
  // char r3procs[10];
  // char nameInt[2];
  //
  // for(i=1; i <= 5; i++){
  //   memset(r3procs, '\0', 10);
  //   strcpy(r3procs, "procsR3_");
  //   itoa(i, nameInt);
  //   strcat(r3procs, nameInt);
  //   struct PCB* PCBnew = setupPCB(r3procs, 1, i);
  //   //suspendPCB(PCBnew->process_name);
  //
  //   cp = (context*)(PCBnew->stackTop);
  //   memset(cp,0, sizeof(context));
  //   cp->fs = 0x10;
  //   cp->gs = 0x10;
  //   cp->ds = 0x10;
  //   cp->es = 0x10;
  //   cp->cs = 0x8;
  //   cp->ebp = (u32int)(PCBnew->stackBase);
  //   cp->esp = (u32int) (PCBnew->stackTop);
  //
  //   //Switch Cases to Run Each Process;
  //   switch(i){
  //     case 1:
  //       cp->eip = (u32int) &proc5;
  //       break;
  //     case 2:
  //       cp->eip = (u32int) &proc4;
  //       break;
  //     case 3:
  //       cp->eip = (u32int) &proc3;
  //       break;
  //     case 4:
  //       cp->eip = (u32int) &proc2;
  //       break;
  //     case 5:
  //       cp->eip = (u32int) &proc1;
  //       break;
  //   }
  //   cp->eflags = 0x202;
  // }
  //
  // checkForRun++;
}
