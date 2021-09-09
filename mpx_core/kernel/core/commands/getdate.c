#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>


int run_getdate() {
  int strlength = 4;
  char daysChar [5] = "\0";
  memset(daysChar,'\0',4);
  char monthChar [5] = "\0";
  memset(monthChar,'\0',4);
  char yearChar [5] = "\0";
  memset(yearChar,'\0',4);
  char milChar [5] = "\0";
  memset(milChar,'\0',4);

  outb(0x70, 0x08);
  int monthVal = inb(0x71);
  int month = (monthVal & 0x0f) + ((monthVal/16)*10);
  itoa(month, monthChar);
  strcat(monthChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,monthChar,&strlength);

  outb(0x70, 0x07);
  int daysVal = inb(0x71);
  int days = (daysVal & 0x0f) + ((daysVal/16)*10);
  itoa(days, daysChar);
  strcat(daysChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,daysChar,&strlength);

  outb(0x70, 0x32);
  int milVal = inb(0x71);
  int mil = (milVal & 0x0f) + ((milVal/16)*10);
  itoa(mil, milChar);
  sys_req(WRITE,DEFAULT_DEVICE,milChar,&strlength);

  outb(0x70, 0x09);
  int yearVal = inb(0x71);
  int year = (yearVal & 0x0f) + ((yearVal/16)*10);
  itoa(year, yearChar);
  strcat(yearChar, "\n");
  sys_req(WRITE,DEFAULT_DEVICE,yearChar,&strlength);

  return 0;
}
