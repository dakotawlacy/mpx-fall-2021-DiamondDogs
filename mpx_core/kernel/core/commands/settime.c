#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/interrupts.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

#include <core/commands/settime.h>

int run_settime(char * commandBuff) {

  //Initialize arrays
  char hours[3];
  char minutes[3];
  char seconds[3];

  if (commandBuff[10] != ' ' || commandBuff[13] != ' ') {
    return -1;
  }

  if(strlen(commandBuff) > 16){
        return 1;
  }

  //Insert values into hours
  hours[0] = commandBuff[8];
  hours[1] = commandBuff[9];
  hours[2] = '\0';

  //Insert values into minutes
  minutes[0] = commandBuff[11];
  minutes[1] = commandBuff[12];
  minutes[2] = '\0';

  //Insert values into seconds
  seconds[0] = commandBuff[14];
  seconds[1] = commandBuff[15];
  seconds[2] = '\0';

  //convert to int
  int hrs = atoi(hours);
  int mins = atoi(minutes);
  int sec = atoi(seconds);

  //Error checking
  if (hrs > 24 || hrs < 0 || mins > 59 || mins < 0 || sec > 59 || sec < 0) {
    return 1;
  }

  //disable interrupts
  cli();

  //Place hour value into register
  outb(0x70, 0x04);
  unsigned char hrBCD = hrs % 10 + ((hrs/10) << 4);
  outb(0x71, hrBCD);

  //Place minutes into register
  outb(0x70, 0x02);
  unsigned char minBCD = mins % 10 + ((mins/10) << 4);
  outb(0x71, minBCD);

  //Place seconds into register
  outb(0x70, 0x00);
  unsigned char secBCD = sec % 10 + ((sec/10) << 4);
  outb(0x71, secBCD);

  //enable interrupts
  sti();

  return 0;
}
