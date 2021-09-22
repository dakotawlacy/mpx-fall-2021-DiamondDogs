#ifndef _STRUCT_H
#define _STRUCT_H

typedef struct PCB{
  char process_name[15];
  int process_class;
  int priority;
  int state;
  unsigned char* process_stack;//1024 bytes
  struct PCB* next;
  struct PCB* previous;

}PCB;

struct queue{
  int count;
  PCB *head;
  PCB *tail;
};

typedef struct queue queue;

struct readyQueue{
  int count;
  int priority;
  PCB *head;
  PCB *tail;
};

typedef struct readyQueue readyQueue;

#endif
