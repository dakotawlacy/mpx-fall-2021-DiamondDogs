#include <core/iocb.h>
#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <system.h>
#include <core/serial.h>
#include <core/struct.h>
#include <core/newserial.h>
#include <core/PCB.h>

que IOCBQueue;

extern int* eflag;

void initIOCBQueue() {
  IOCBQueue.head = NULL;
  IOCBQueue.head->next = NULL;
}

void createIOCB(u32int address, char* buffer, int* buffer_length, int operation,int* eflag) {

  //Allocate IOCB
  struct IOCB* new = sys_alloc_mem(sizeof(IOCB));

  new->process = address;
  new->buffer = buffer;
  new->buffer_length = buffer_length;
  new->writeread = operation;
  new->eventFlag = eflag;
  new->next = NULL;

  insertIOCB(new);

}

void insertIOCB(struct IOCB* new) {

  //Check if head is null
  if (IOCBQueue.head == NULL) {
    //Place IOCB at head
    IOCBQueue.head = new;
    new->next = NULL;

    return;
  }

  //Traverse to end of list
  IOCB* curr = IOCBQueue.head;
  while (curr->next != NULL) {
    curr = curr->next;
  }
  //Set last IOCB
  curr->next = new;
  new->next = NULL;

  return;

}

void IOCBScheduler() {


    //Check if queue is empty
    if (IOCBQueue.head == NULL) {
      return;
    }

    //Check to see if head is completed
    if (*(IOCBQueue.head->eventFlag) == 1) {

      //Remove from queue
      //IOCB* iocbToFree = IOCBQueue.head;
      //PCB* next = (PCB*)IOCBQueue.head->next->process;
      PCB* head = (PCB*)IOCBQueue.head->process;

      //serial_println(next->process_name);

      //Remove from blocked, place into ready
      removePCB(head);
      head->state = 1;
      insertPCB(head);
      *(IOCBQueue.head->eventFlag) = 0;




      IOCBQueue.head = IOCBQueue.head->next;

      return;

    }
    else
    {
      //Complete request
      if (IOCBQueue.head->writeread == 0) {
        //Write
        com_write(IOCBQueue.head->buffer,IOCBQueue.head->buffer_length);
      }
      else if (IOCBQueue.head->writeread == 1) {
        //Read
        com_read(IOCBQueue.head->buffer,IOCBQueue.head->buffer_length);
      }
    }
}
