#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <include/string.h>


int run_gettime() {
  int str_length = 4;
  char secChar [5] = "\0";
  memset(secChar,'\0',4);
  char hoursChar [5] = "\0";
  memset(hoursChar,'\0',4);
  char minutesChar [5] = "\0";
  memset(minutesChar,'\0',4);

  outb(0x70, 0x04);
  int hoursVal = inb(0x71);
  int hours = (hoursVal & 0x0f) + ((hoursVal/16)*10);
  itoa(hours, hoursChar);
  strcat(hoursChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,hoursChar,&str_length);

  outb(0x70, 0x02);
  int minutesVal = inb(0x71);
  int minutes = (minutesVal & 0x0f) + ((minutesVal/16)*10);
  itoa(minutes, minutesChar);
  strcat(minutesChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,minutesChar,&str_length);

  outb(0x70, 0x00);
  int secondsVal = inb(0x71);
  int seconds = (secondsVal & 0x0f) + ((secondsVal/16)*10);
  itoa(seconds, secChar);
  strcat(secChar, "\n");
  sys_req(WRITE,DEFAULT_DEVICE,secChar,&str_length);
  return 0;
}
