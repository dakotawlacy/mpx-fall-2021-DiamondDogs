/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.

      Initial Kernel -- by Forrest Desjardin, 2013,
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
      				Ben Smith 2018, and Alex Wilson 2019
*/


#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/command_handler.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include <core/startup.h>
#include "modules/mpx_supt.h"
#include <core/struct.h>
#include <core/PCB.h>
#include <core/mcb.h>
#include <core/commands/yield.h>
#include <core/newserial.h>
#include <core/iocb.h>
#include <core/newprocesses.h>

int* eflag;

void kmain(void)
{
   init_serial(COM1);
   set_serial_in(COM1);
   set_serial_out(COM1);

   extern uint32_t magic;
   // Uncomment if you want to access the multiboot header
   // extern void *mbd;
   // char *boot_loader_name = (char*)((long*)mbd)[16];


   // 0) Initialize Serial I/O
   // functions to initialize serial I/O can be found in serial.c
   // there are 3 functions to call
   klogv("Starting MPX boot sequence...");
   klogv("Initialized serial I/O on COM1 device...");

   //     MPX Module.  This will change with each module.
   // 1) Initialize the support software by identifying the current
   // you will need to call mpx_init from the mpx_supt.c
   mpx_init(IO_MODULE);
   mpx_init(MEM_MODULE);
   // 2) Check that the boot was successful and correct when using grub
   // Comment this when booting the kernel directly using QEMU, etc.
   if ( magic != 0x2BADB002 ){
     //kpanic("Boot was not error free. Halting.");
   }

   // 3) Descriptor Tables -- tables.c
   //  you will need to initialize the global
   // this keeps track of allocated segments and pages
   klogv("Initializing descriptor tables...");
   init_gdt();


    // 4)  Interrupt vector table --  tables.c
    // this creates and initializes a default interrupt vector table
    // this function is in tables.c
    init_idt();

        // 4)  Interrupt vector table --  tables.c
        // this creates and initializes a default interrupt vector table
        // this function is in tables.c
        init_pic();
        init_irq();
        sti();
        klogv("Interrupt vector table initialized!");

       // 5) Virtual Memory -- paging.c  -- init_paging
       //  this function creates the kernel's heap
       //  from which memory will be allocated when the program calls
       // sys_alloc_mem UNTIL the memory management module  is completed
       // this allocates memory using discrete "pages" of physical memory
       // NOTE:  You will only have about 70000 bytes of dynamic memory
       //
       init_paging();
       klogv("Initializing virtual memory...");

       sys_set_malloc(&allocateMem);
       sys_set_free(&freeMem);
       initHeap();

       // com_open(1200);
       // int a = 1;
       // while(1) {
       //   (void) a;
       // }

       // 6) Call YOUR command handler -  interface method
       initQueues();
       initIOCBQueue();

       klogv("Transferring control to commhand...");



       //Command Handler Process
       PCB* newPCB = setupPCB("command_handler",1,9);
       context* cp = (context*) newPCB->stackTop;
       memset(cp,0,sizeof(context));
       cp->fs = 0x10;
       cp->gs = 0x10;
       cp->ds = 0x10;
       cp->es = 0x10;
       cp->cs = 0x8;
       cp->ebp = (u32int)(newPCB->stackBase);
       cp->esp = (u32int) (newPCB->stackTop);
       cp->eip = (u32int) &run_ch;
       cp->eflags = 0x202;

       removePCB(newPCB);
       newPCB->susState = 0;
       insertPCB(newPCB);

       //Idle Process
       newPCB = setupPCB("IDLE",1,0);
       cp = (context*) newPCB->stackTop;
       memset(cp,0,sizeof(context));
       cp->fs = 0x10;
       cp->gs = 0x10;
       cp->ds = 0x10;
       cp->es = 0x10;
       cp->cs = 0x8;
       cp->ebp = (u32int)(newPCB->stackBase);
       cp->esp = (u32int) (newPCB->stackTop);
       cp->eip = (u32int) &idle;
       cp->eflags = 0x202;

       removePCB(newPCB);
       newPCB->susState = 0;
       insertPCB(newPCB);

       newPCB = setupPCB("COMWRITE",1,5);
       cp = (context*) newPCB->stackTop;
       memset(cp,0,sizeof(context));
       cp->fs = 0x10;
       cp->gs = 0x10;
       cp->ds = 0x10;
       cp->es = 0x10;
       cp->cs = 0x8;
       cp->ebp = (u32int)(newPCB->stackBase);
       cp->esp = (u32int) (newPCB->stackTop);
       cp->eip = (u32int) &comwrite_test;
       cp->eflags = 0x202;

       removePCB(newPCB);
       newPCB->susState = 0;
       insertPCB(newPCB);

       //int* eflag = sys_alloc_mem(sizeof(int*));

       com_open(1200,eflag);

       run_yield();

       com_close();
       // 7) System Shutdown on return from your command handler
       klogv("Starting system shutdown procedure...");

       /* Shutdown Procedure */
       klogv("Shutdown complete.");

}
