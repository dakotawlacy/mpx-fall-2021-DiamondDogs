#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/interrupts.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

#include <core/commands/settime.h>

int run_settime(char * commandBuff) {

  char hours[3];
  char minutes[3];
  char seconds[3];
    hours[0] = commandBuff[8];
    hours[1] = commandBuff[9];
    hours[2] = '\0';

    minutes[0] = commandBuff[11];
    minutes[1] = commandBuff[12];
    minutes[2] = '\0';

    seconds[0] = commandBuff[14];
    seconds[1] = commandBuff[15];
    seconds[2] = '\0';

      int hrs = atoi(hours);
      int mins = atoi(minutes);
      int sec = atoi(seconds);

      cli();

      outb(0x70, 0x04);
      unsigned char hrBCD = hrs % 10 + ((hrs/10) << 4);
      outb(0x71, hrBCD);

      outb(0x70, 0x02);
      unsigned char minBCD = mins % 10 + ((mins/10) << 4);
      outb(0x71, minBCD);

      outb(0x70, 0x00);
      unsigned char secBCD = sec % 10 + ((sec/10) << 4);
      outb(0x71, secBCD);

      sti();
  return 0;
}
