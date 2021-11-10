#ifndef _STRUCT_H
#define _STRUCT_H

typedef struct PCB {

  char process_name[15];
  int process_class;
  int priority;
  int state;
  int susState;
  unsigned char process_stack[1024];//1024 bytes
  unsigned char* stackBase;
  unsigned char* stackTop;
  struct PCB* next;
  struct PCB* previous;
  struct context* cont;

}PCB;

typedef struct MCB {

  int type;//0 for free, 1 for allocated
  u32int address;
  int size;
  struct MCB* next;
  struct MCB* prev;

}MCB;

typedef struct queue{
  int count;
  PCB *head;
  PCB *tail;
}queue;

typedef struct list {

  int count;
  MCB *head;

}list;

typedef struct context {
  u32int gs, fs, es, ds;
  u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  u32int eip, cs, eflags;
}context;


#endif
