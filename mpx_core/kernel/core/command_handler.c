#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/command_handler.h>




int run_ch() {

  //Welcome Message and Length
  char* welcomeMessage = "Welcome to DiamondDogs OS\n";
  int welcome_length = strlen(welcomeMessage);

  //Help Message and_Length
  char* helpMessage = "Type 'help' to list all commands\n";
  int help_length = strlen(welcomeMessage);

  //Write Welcome Message
   sys_req(WRITE,DEFAULT_DEVICE,welcomeMessage,&welcome_length);

   //Write Help Message
    sys_req(WRITE,DEFAULT_DEVICE,helpMessage,&help_length);


   char commandBuff[100];
   int bufferSize = 99;
   memset(&commandBuff,'\0',100);


   sys_req(READ,DEFAULT_DEVICE,commandBuff,&bufferSize);

   serial_println("\nComHand Below");
   serial_print(commandBuff);
   if(strcmp(commandBuff, "hello") == 0){
     yeet();
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


int yeet(){
  serial_println("It works");
  return 0;
}
