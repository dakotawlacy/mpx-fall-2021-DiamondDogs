#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <include/string.h>

void run_gettime() {

  //Initialize char arrays
  int str_length = 4;
  int leadingZero = 2;
  char secChar [5] = "\0";
  memset(secChar,'\0',4);
  char hoursChar [5] = "\0";
  memset(hoursChar,'\0',4);
  char minutesChar [5] = "\0";
  memset(minutesChar,'\0',4);

  //Read hours value from register, convert, and print
  outb(0x70, 0x04);
  int hoursVal = inb(0x71);
  int hours = (hoursVal & 0x0f) + ((hoursVal/16)*10);


  if (hours < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }
  //serial_println(hr);

  itoa(hours, hoursChar);
  strcat(hoursChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,hoursChar,&str_length);

  //Read minutes value from register, convert, and print
  outb(0x70, 0x02);
  int minutesVal = inb(0x71);
  int minutes = (minutesVal & 0x0f) + ((minutesVal/16)*10);

  if (minutes < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(minutes, minutesChar);
  strcat(minutesChar, ":");
  sys_req(WRITE,DEFAULT_DEVICE,minutesChar,&str_length);

  //Read seconds value from register, convert, and print
  outb(0x70, 0x00);
  int secondsVal = inb(0x71);
  int seconds = (secondsVal & 0x0f) + ((secondsVal/16)*10);

  if (seconds < 10){
    sys_req(WRITE,DEFAULT_DEVICE,"0\0",&leadingZero);
  }

  itoa(seconds, secChar);
  strcat(secChar, "\n");
  sys_req(WRITE,DEFAULT_DEVICE,secChar,&str_length);




  return;


}
