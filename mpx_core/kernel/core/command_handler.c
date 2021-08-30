#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/command_handler.h>




int run_ch() {

  char commandBuff[100];
  
  char buffer;

  serial_println("Welcome to DiamondDogs OS");
  serial_println("What command would you like to run?");

  list_commands();

   int bufferSize = 99;
   memset(&buffer,'\0',100);


   if (sys_req(READ,DEFAULT_DEVICE,commandBuff,&bufferSize) == 2000) {
     serial_println("test1");
   }

  return 0;

}

int list_commands() {

  serial_println("1. Version");
  serial_println("2. Help");
  serial_println("3. Shutdown");
  serial_println("4. Date and Time");

  return 0;
}
