#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/struct.h>

typedef struct PCB{
  char process_name[15];
  int process_class;
  int priority;
  int state;
  unsigned char* process_stack;//1024 bytes
  PCB *next;
  PCB *previous;

}

typedef struct queue{
  int count;
  PCB *head;
  PCB *tail;
}
