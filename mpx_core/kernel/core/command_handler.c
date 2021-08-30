#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/command_handler.h>
#include <modules/mpx_supt.h>


int run_ch() {

  ch_running = 1;
  char commandBuff[100];
  int commandSize;
  mpx_init(MODULE_R1);
  
  serial_println("Welcome to DiamondDogs OS");
  serial_println("What command would you like to run?");

  list_commands();

  WHILE(CH_RUNNING) {

    commandSize = 99;
    memset (buffer,'\0',100);
    sys_req(READ,DEFAULT_DEVICE,commandBuff,&commandSize);

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
