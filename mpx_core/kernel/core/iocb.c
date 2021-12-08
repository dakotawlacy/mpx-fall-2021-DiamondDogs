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

void initIOCBQueue() {
  IOCBQueue.head = NULL;
}

void createIOCB(u32int address, char* buffer, int* buffer_length, int operation) {

  struct IOCB* new = sys_alloc_mem(sizeof(struct IOCB));

  new->process = address;
  new->buffer = buffer;
  new->buffer_length = buffer_length;
  new->writeread = operation;
  new->eventFlag = 0;

  insertIOCB(new);

}

void insertIOCB(struct IOCB* new) {

  if (IOCBQueue.head == NULL) {
    IOCBQueue.head = new;
    new->next = NULL;

    return;
  }

  IOCB* curr = IOCBQueue.head;
  while (curr->next != NULL) {
    curr = curr->next;
  }

  curr->next = new;
  new->next = NULL;

  return;

}

void IOCBScheduler() {

    if (IOCBQueue.head == NULL) {
      return;
    }

    //check for completion
    if (IOCBQueue.head->eventFlag) {
      //Remove from queue
      IOCB* iocbToFree = IOCBQueue.head;
      IOCB* iocbNext = IOCBQueue.head->next;

      PCB* head = (PCB*)iocbToFree->process;

      removePCB(head);
      head->state = 1;
      insertPCB(head);

      IOCBQueue.head = iocbNext;

      return;
    }

    //Complete request
    if (IOCBQueue.head->writeread == 0) {
      //Write
      IOCBQueue.head->eventFlag = com_write(IOCBQueue.head->buffer,IOCBQueue.head->buffer_length);

    }
    else if (IOCBQueue.head->writeread == 1) {
      //Read
      IOCBQueue.head->eventFlag = com_read(IOCBQueue.head->buffer,IOCBQueue.head->buffer_length);

    

    }


}
