#ifndef _DCB_H
#define _DCB_H

typedef struct DCB {

  int open;
  int event;
  int status;//0 idle, 1 write, 2 read

  char* device_buffer;
  int* count_ptr;
  int device_buffer_index;

  char ringbuff[99];
  int ringbuff_size;
  int ringbuff_index;

}DCB;


#endif
