#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_getdate() {

  //String length of command
  int strlength = 4;
  int leadingZero = 2;

  //Initialize char arrays
  char daysChar [5] = "\0";
  memset(daysChar,'\0',4);
  char monthChar [5] = "\0";
  memset(monthChar,'\0',4);
  char yearChar [5] = "\0";
  memset(yearChar,'\0',4);
  char milChar [5] = "\0";
  memset(milChar,'\0',4);

  //Read month value from register, convert, and print
  outb(0x70, 0x08);
  int monthVal = inb(0x71);
  int month = (monthVal & 0x0f) + ((monthVal/16)*10);

  if (month < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(month, monthChar);
  strcat(monthChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,monthChar,&strlength);

  //Read day value from register, convert, and print
  outb(0x70, 0x07);
  int daysVal = inb(0x71);
  int days = (daysVal & 0x0f) + ((daysVal/16)*10);

  if (days < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(days, daysChar);
  strcat(daysChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,daysChar,&strlength);

  //Read millenium value from register, convert, and print
  outb(0x70, 0x32);
  int milVal = inb(0x71);
  int mil = (milVal & 0x0f) + ((milVal/16)*10);

  if (mil < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(mil, milChar);
  sys_req(WRITE,DEFAULT_DEVICE,milChar,&strlength);

  //Read decade from register, convert, and print
  outb(0x70, 0x09);
  int yearVal = inb(0x71);
  int year = (yearVal & 0x0f) + ((yearVal/16)*10);
  if (year < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(year, yearChar);
  strcat(yearChar, "\n");
  sys_req(WRITE,DEFAULT_DEVICE,yearChar,&strlength);

  return 0;
}
