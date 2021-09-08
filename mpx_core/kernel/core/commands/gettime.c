#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <include/string.h>
<<<<<<< HEAD
=======

int base = 10;
>>>>>>> fff27fecd5d788a8ebf19b06ac9a094a8c1ecb5f

int run_gettime() {
  char* secChar = "\0";
  memset(secChar,'\0',99);
  char* hoursChar = "\0";
  memset(hoursChar,'\0',99);
  char* minutesChar = "\0";
  memset(minutesChar,'\0',99);

  outb(0x70, 0x04);
  int hoursVal = inb(0x71);
  int hours = (hoursVal & 0x0f) + ((hoursVal/16)*10);
  itoa(hours, hoursChar);
<<<<<<< HEAD
  serial_print(hoursChar);

=======

  serial_print("\nTime: ");
  serial_print(hoursChar);

  if (base == 20){
    serial_println("Not Okay");
  }




  serial_print(":");

>>>>>>> fff27fecd5d788a8ebf19b06ac9a094a8c1ecb5f

  outb(0x70, 0x02);
  int minutesVal = inb(0x71);
  int minutes = (minutesVal & 0x0f) + ((minutesVal/16)*10);
  itoa(minutes, minutesChar);
  serial_print(minutesChar);
<<<<<<< HEAD
  
=======
  serial_print(":");


>>>>>>> fff27fecd5d788a8ebf19b06ac9a094a8c1ecb5f

  outb(0x70, 0x00);
  int secondsVal = inb(0x71);
  int seconds = (secondsVal & 0x0f) + ((secondsVal/16)*10);
  itoa(seconds, secChar);
  serial_println(secChar);
  return 0;
}
