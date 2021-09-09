#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/interrupts.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/commands/setdate.h>

int run_setdate(char * commandBuff) {

  //Initialize array
  char days[3];
  char month[3];
  char yearLSB[3];
  char yearMSB[3];

  //Copy month
  month[0] = commandBuff[8];
  month[1] = commandBuff[9];
  month[2] = '\0';

  //Copy day
  days[0] = commandBuff[11];
  days[1] = commandBuff[12];
  days[2] = '\0';

  //Copy century
  yearMSB[0] = commandBuff[14];
  yearMSB[1] = commandBuff[15];
  yearMSB[2] = '\0';

  //Copy decade
  yearLSB[0] = commandBuff[16];
  yearLSB[1] = commandBuff[17];
  yearLSB[2] = '\0';

  //Convert string to num
  int monthNum = atoi(month);
  int dayNum = atoi(days);
  int yearMSBNum = atoi(yearMSB);
  int yearLSBNum = atoi(yearLSB);

  //Disable interrupt
  cli();

  //Convert month int to BCD and place into register
  outb(0x70, 0x08);
  unsigned char monthBCD = monthNum % 10 + ((monthNum/10) << 4);
  outb(0x71, monthBCD);

  //Convert day int to BCD and place into register
  outb(0x70, 0x07);
  unsigned char dayBCD = dayNum % 10 + ((dayNum/10) << 4);
  outb(0x71, dayBCD);

  //Convert year int to BCD and place into register
  outb(0x70, 0x09);
  unsigned char yearlBCD = yearLSBNum % 10 + ((yearLSBNum/10) << 4);
  outb(0x71, yearlBCD);

  outb(0x70, 0x32);
  unsigned char yearmBCD = yearMSBNum % 10 + ((yearMSBNum/10) << 4);
  outb(0x71, yearmBCD);

  //Reenable interrupt
  sti();

  return 0;
}
