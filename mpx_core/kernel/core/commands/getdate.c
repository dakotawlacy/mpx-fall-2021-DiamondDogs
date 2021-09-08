#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_getdate() {
  char* daysChar = "\0";
  memset(daysChar,'\0',99);
  char* monthChar = "\0";
  memset(monthChar,'\0',99);
  char* yearChar = "\0";
  memset(yearChar,'\0',99);

  outb(0x70, 0x08);
  int monthVal = inb(0x71);
  int month = (monthVal & 0x0f) + ((monthVal/16)*10);
  itoa(month, monthChar);
  serial_print(monthChar);

  outb(0x70, 0x07);
  int daysVal = inb(0x71);
  int days = (daysVal & 0x0f) + ((daysVal/16)*10);
  itoa(days, daysChar);
  serial_print(daysChar);

  outb(0x70, 0x09);
  int yearVal = inb(0x71);
  int year = (yearVal & 0x0f) + ((yearVal/16)*10);
  itoa(year, yearChar);
  serial_print(yearChar);

  return 0;
}
