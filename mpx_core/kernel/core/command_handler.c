kt#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>

#include <core/command_handler.h>

#include <core/commands/help.h>
#include <core/commands/shutdown.h>
#include <core/commands/version.h>
#include <core/commands/getdate.h>

#include <core/commands/setdate.h>
#include <core/commands/gettime.h>
#include <core/commands/settime.h>

//Run the Command Handler function
int run_ch() {

  // Welcome Message and Length
  char * welcomeMessage = "Welcome to DiamondDogs OS\n";
  int welcome_length = strlen(welcomeMessage);

  // Command Message and Length
  char * helpMessage = "Type 'help' to list all commands\n";
  int helpMessage_length = strlen(helpMessage);

  // Write welcome message
  sys_req(WRITE,DEFAULT_DEVICE,welcomeMessage,&welcome_length);

   //Write Help Message
   sys_req(WRITE,DEFAULT_DEVICE,helpMessage,&helpMessage_length);


   char commandBuff[100];
   int bufferSize = 99;
   memset(&commandBuff,'\0',100);

   while(1){
   sys_req(READ,DEFAULT_DEVICE,commandBuff,&bufferSize);

   if (get_command(commandBuff, bufferSize) > 0){
      break;
   }

   while(bufferSize >= 0){
     commandBuff[bufferSize] = '\0';
     bufferSize--;
   }
   bufferSize = 99;


  }

  return 0;

}


int get_command(char * commandBuff, int bufferSize) {

  int i = 0;
  char command[99];
  //Copy first word into command
  for(i = 0; i < strlen(commandBuff); i++) {
    if (commandBuff[i] == ' ' || commandBuff[i] == '\0') {
      break;
    } else {
      command[i] = commandBuff[i];
    }
  }

  //Command Logic
  if (strcmp(command,"help") == 0) {
    //Run Help
    serial_println("help");
    run_help();
  }

  if (strcmp(command,"version") == 0) {
    //Run version
    serial_println("version");
    run_version();
  }

  if (strcmp(command,"shutdown") == 0) {
    //Run shutdown
    //serial_println("shutdown");
    return run_shutdown(commandBuff, bufferSize);
  }

  if (strcmp(command,"getdate") == 0) {
    //Run getdate
    serial_println("getdate");
    run_getdate();
  }

  if (strcmp(command,"setdate") == 0) {
    //Run setdate
    serial_println("setdate");
    run_setdate();
  }

  if (strcmp(command,"gettime") == 0) {
    //Run gettime
    serial_println("gettime");
    run_gettime();
  }

  if (strcmp(command,"settime") == 0) {
    //Run settime
    serial_println("settime");
    run_settime();
  }

  return 0;
}
