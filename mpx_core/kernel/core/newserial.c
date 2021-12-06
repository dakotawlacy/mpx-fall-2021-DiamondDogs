#include <core/newserial.h>
#include <system.h>
#include <core/dcb.h>
#include <core/serial.h>
#include <core/io.h>
#include <core/tables.h>
#include "string.h"

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

  if (serial_dcb.open) {

    cli();

    int type = inb(dev + 2);
    int bit1 = type>>1 & 1;
    int bit2 = type>>2 & 1;

    if (!bit1 && !bit2) {
      //Modem
      inb(dev + 6);
    }
    else if (!bit1 && bit2) {
      //output handler
      klogv("output");

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
  serial_dcb.event = 1;//done?
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

  return 0;

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

    set_int(1,0);

  }

  if (serial_dcb.device_buffer_index < *(serial_dcb.count_ptr)) {
    outb(COM1,*(serial_dcb.device_buffer));
    serial_dcb.device_buffer++;
    serial_dcb.device_buffer_index++;

  }
}
