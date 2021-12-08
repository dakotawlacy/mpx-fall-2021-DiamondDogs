#ifndef _IOCB_H
#define _IOCB_H

#include <system.h>

typedef struct IOCB {

  u32int process;
  int* eventFlag;//0 not finished, 1 finished
  int writeread;//0 write, 1 read;
  char* buffer;
  int* buffer_length;
  struct IOCB* next;
  struct IOCB* prev;


}IOCB;

typedef struct que {
  IOCB* head;
}que;


void initIOCBQueue();

void createIOCB(u32int address, char* buffer, int* buffer_length, int operation,int* eflag);

void insertIOCB(struct IOCB* iocb);

void IOCBScheduler();

#endif
