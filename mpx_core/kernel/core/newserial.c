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

void set_int(int bit, int on) {

  if (on) {
    outb(dev + 1, inb(dev + 1) | (1<<bit));
  }
  else {
    outb(dev + 1, inb(dev + 1) & ~(1<<bit));
  }
}

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

int com_open(int baud_rate) {

  cli();

  serial_dcb.open = 1;//open
  serial_dcb.event = 0;//done?
  serial_dcb.status = 0;//idle, read, write

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

int com_write(char* buffer, int* count) {

  //Error check in future;
  serial_dcb.device_buffer = buffer;
  serial_dcb.count_ptr = count;
  serial_dcb.status = 1;//Set to write
  serial_dcb.device_buffer_index = 0;
  serial_dcb.event = 0;

  set_int(1,1);

  outb(COM1,serial_dcb.device_buffer[serial_dcb.device_buffer_index]);

  serial_dcb.device_buffer_index++;
  serial_dcb.device_buffer++;

  sti();

  return serial_dcb.event;

}

void write_interrupt() {

  //check to see if writing
  if (serial_dcb.status != 1) {
    return;
  }

  //Check to see if we're at end of buffer
  if (serial_dcb.device_buffer_index == *(serial_dcb.count_ptr)) {
    serial_dcb.status = 0;//set status to idle
    serial_dcb.event = 1;//set event flag
    *(serial_dcb.count_ptr) = serial_dcb.device_buffer_index;

    set_int(1,1);

  }

  if (serial_dcb.device_buffer_index < *(serial_dcb.count_ptr)) {
    outb(COM1,*(serial_dcb.device_buffer));
    serial_dcb.device_buffer++;
    serial_dcb.device_buffer_index++;

  }


}

int com_read(char* buffer, int* count) {

  serial_dcb.device_buffer = buffer;
  serial_dcb.count_ptr = count;
  serial_dcb.status = 2;//READ
  serial_dcb.device_buffer_index = 0;
  serial_dcb.event = 0;
  cli();

  set_int(1,1);

  // while (serial_dcb.ringbuff[serial_dcb.ringbuff_index] != '\n' &&
  //         serial_dcb.device_buffer_index < *(serial_dcb.count_ptr) &&
  //         serial_dcb.ringbuff_size > 0)
  //         {
  //   serial_dcb.device_buffer[serial_dcb.device_buffer_index] = serial_dcb.ringbuff[serial_dcb.ringbuff_index];
  //   serial_dcb.ringbuff[serial_dcb.ringbuff_index] = '\0';
  //   serial_dcb.ringbuff_index++;
  //   serial_dcb.device_buffer_index++;
  //   serial_dcb.ringbuff_size--;
  //
  //   if (serial_dcb.ringbuff_index >= 99) {
  //     serial_dcb.ringbuff_index = 0;
  //   }
  // }

  if (serial_dcb.device_buffer_index < *(serial_dcb.count_ptr)) {
    return 0;
  }


  *(serial_dcb.count_ptr) = serial_dcb.device_buffer_index;
  serial_dcb.status = 0;
  serial_dcb.event = 1;

  set_int(1,0);

  sti();

  return 1;

}

void read_interrupt() {

  char letter;
  letter = inb(dev);
  //outb(COM1,letter);

  if (serial_dcb.status == 2) {//Check for read

    if (letter == '\r' || letter == '\n' || serial_dcb.device_buffer_index == *(serial_dcb.count_ptr)) {

      serial_dcb.ringbuff[serial_dcb.ringbuff_index] = letter;//Get ending char
      serial_dcb.ringbuff_size++; //Increase ring buffer size
      serial_dcb.ringbuff_index++;//Increase ring buffer index
      *(serial_dcb.count_ptr) = serial_dcb.device_buffer_index;

      serial_println(serial_dcb.ringbuff);

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
