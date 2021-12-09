#include <core/newserial.h>
#include <system.h>
#include <core/dcb.h>
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include "string.h"
#include <core/keyboard_capture.h>

u32int dev = COM1;
//
int level = 4;

struct DCB serial_dcb;

u32int original_idt_entry;

/*
  Procedure: set_int
  Description: Allows the interrupt to be set so
              the correct functions will be used
              when called.
  Params: (int) bit, (int) on
*/
void set_int(int bit, int on) {

  if (on) {
    outb(dev + 1, inb(dev + 1) | (1<<bit));
  }
  else {
    outb(dev + 1, inb(dev + 1) & ~(1<<bit));
  }
}

/*
  Procedure: top_handle
  Description: Takes in values and determines
    where to redirect them so the machine can
    continue running efficiently.
  Params; N/A
*/
void top_handle() {

  //Check to see if its open
  if (serial_dcb.open) {

    cli();

    //Get what line
    int type = inb(dev + 2);
    int bit1 = type>>1 & 1;
    int bit2 = type>>2 & 1;


    if (!bit1 && !bit2) {
      //Modem
      inb(dev + 6);
    }
    else if (!bit1 && bit2) {
      //output handler
      read_interrupt();

    }
    else if (bit1 && !bit2) {
      //input handler
      write_interrupt();

    }
    else if (bit1 && bit2) {
      inb(dev + 5);
    }

      sti();

      outb(0x20, 0x20);

  }
}

/*
  Procedure: com_open
  Description: Allows for communication to decide what the
    machine is going to do next.
  Params: (int) baud_rate
*/
int com_open(int baud_rate,int* eflag) {

  cli();

  serial_dcb.open = 1;//open
  serial_dcb.event = eflag;//done?
  serial_dcb.status = 0;//idle, read, write
  serial_dcb.done = 0;

  serial_dcb.device_buffer = NULL;
  serial_dcb.count_ptr = NULL;
  serial_dcb.device_buffer_index = 0;

  memset(serial_dcb.ringbuff, '\0', 99);
  serial_dcb.ringbuff_size = 0;
  serial_dcb.ringbuff_index = 0;

  original_idt_entry = idt_get_gate(0x24);
  idt_set_gate(0x24, (u32int)top_handle, 0x08, 0x8e);

  long brd = 1115200/(long) baud_rate;

  outb(dev + 1, 0b00000000);//disable interrupt

  outb(dev + 3, 0b10000000);//st line control

  outb(dev + 0, brd);//st bsd lsb

  outb(dev + 1, brd >> 8);//msb bsd

  outb(dev + 3, 0b00000011);//lock divisior

  outb(dev + 2, 0b11000111);//enable fifo

  outb(0x21, inb(0x21) & ~(1<<level));

  outb(dev + 1, 0b00000001);//enable interrupt

  (void) inb(dev);

  sti();

  return 0;
}

int com_close() {

  set_int(1,0);
  outb(0x21, inb(0x21) & ~(1<<level));
  set_int(1,1);

  outb(dev+1,0x00);
  outb(dev+6,0x00);

  return 0;

}

/*
  Procedure: com_write
  Description: Allows you to write to the screen (output)
  Params: (char*) buffer, (int*) count
*/
int com_write(char* buffer, int* count) {

  if (buffer == NULL) {
    return -402;
  }

  if (count == NULL) {
    return -403;
  }

  if (serial_dcb.status != 0) {
    return -404;
  }

  serial_dcb.device_buffer = buffer;
  serial_dcb.count_ptr = count;
  serial_dcb.status = 1;//Set to write
  serial_dcb.device_buffer_index = 0;

  *(serial_dcb.event) = 0;

  int en;
  en = inb(dev+1);
  en = en | 0x02;
  outb(dev+1,en);

  outb(COM1,serial_dcb.device_buffer[serial_dcb.device_buffer_index]);

  serial_dcb.device_buffer_index++;
  serial_dcb.device_buffer++;

  sti();

  return 0;

}

/*
    Procedure: write_interrupt
    Description: Calls the interrupt that allows the
                 machine to write out what is collected in the buffer.

    "Puts in Write Mode"
    Params: N/A
*/
void write_interrupt() {

  //check to see if writing
  if (serial_dcb.status != 1) {
    return;
  }

  //Check to see if we're at end of buffer
  if (serial_dcb.device_buffer_index == *(serial_dcb.count_ptr)) {
    serial_dcb.status = 0;//set status to idle
    *(serial_dcb.event) = 1;//set event flag
    *(serial_dcb.count_ptr) = serial_dcb.device_buffer_index;

    int en;
    en = inb(dev+1);
    en = en & ~0x02;
    outb(dev+1,en);

  }

  if (serial_dcb.device_buffer_index < *(serial_dcb.count_ptr)) {
    outb(COM1,*(serial_dcb.device_buffer));
    serial_dcb.device_buffer++;
    serial_dcb.device_buffer_index++;
  }
}

/*
  Procedure: com_read
  Description: Reads in the input the user inputs
  Params: (char*) buffer, (int*) count
*/
int com_read(char* buffer, int* count) {

  serial_dcb.device_buffer = buffer;
  serial_dcb.count_ptr = count;
  serial_dcb.status = 2;//READ
  serial_dcb.device_buffer_index = 0;
  *(serial_dcb.event) = 0;

  cli();

  set_int(1,1);

  if (!serial_dcb.done) {
    return 0;
  }

  int i = 0;
  while (i < 99) {

    serial_dcb.device_buffer[i] = serial_dcb.ringbuff[i];
    serial_dcb.ringbuff[serial_dcb.device_buffer_index] = '\0';
    //serial_dcb.device_buffer_index++;

    i++;
    serial_dcb.ringbuff_size--;

  }

  serial_dcb.done = 0;
  memset(serial_dcb.ringbuff, '\0', 99);
  serial_dcb.ringbuff_size = 0;
  serial_dcb.ringbuff_index = 0;


  serial_dcb.status = 0;//Idle
  *(serial_dcb.event) = 1;//Done

  set_int(1,0);

  sti();

  return 1;

}

/*
  Procedure: read_interrupt
  Description: Calls the interrupt that allows the machine to read in the input.

  "Puts in Read Mode"

  Params: N/A
*/
void read_interrupt() {

  char letter;
  letter = inb(dev);
  outb(COM1,letter);

  if (serial_dcb.status == 2) {//Check for read

    if (letter == '\r' || letter == '\n' || serial_dcb.device_buffer_index == *(serial_dcb.count_ptr)) {

      serial_dcb.ringbuff[serial_dcb.ringbuff_index] = '\0';//Get ending char
      serial_dcb.ringbuff_size++; //Increase ring buffer size
      serial_dcb.ringbuff_index++;//Increase ring buffer index
      *(serial_dcb.count_ptr) = serial_dcb.ringbuff_index;

      serial_dcb.done = 1;

      return;
    }

    serial_dcb.ringbuff[serial_dcb.ringbuff_index] = letter;//Place letter in ring buffer
    serial_dcb.ringbuff_size++; //Increase ring buffer size
    serial_dcb.ringbuff_index++;//Increase ring buffer index

    if (serial_dcb.ringbuff_index > 99) {
      serial_dcb.ringbuff_index = 0;//Reset ring buffer index
    }

    return;

  }

  //If not read
  serial_dcb.device_buffer[serial_dcb.device_buffer_index] = letter;
  serial_dcb.device_buffer_index++;

}
