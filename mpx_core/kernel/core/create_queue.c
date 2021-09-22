#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/struct.h>
#include <core/create_queue.h>

void createQ(){
  queue q1;
  readyQueue rQ;
  ready_initialize(&rQ);
  blocked_initialize(&q1);
}

void ready_initialize(readyQueue *q){
  q->count = 0;
  q->priority = 0;
  q->head = NULL;
  q->tail = NULL;
}

void blocked_initialize(queue *q){
  q->count = 0;
  q->head = NULL;
  q->tail = NULL;
}
