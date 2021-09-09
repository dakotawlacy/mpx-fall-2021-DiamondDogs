#include <stdint.h>
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

  //Char array for commands
  char commandBuff[100];
  int bufferSize = 99;
  memset(&commandBuff,'\0',100);

  //Command Handler Loop
  while(1) {

    //Get input
    sys_req(READ,DEFAULT_DEVICE,commandBuff,&bufferSize);

    //Check for shutdown command
    if (get_command(commandBuff, bufferSize) > 0){
      break;
    }

    //Reset buffer
    while(bufferSize >= 0){
      commandBuff[bufferSize] = '\0';
      bufferSize--;
    }
     bufferSize = 99;
   }

  return 0;

}

//Parse command
int get_command(char * commandBuff, int bufferSize) {

  //Declare variables
  int i = 0;
  int newLine = 2;
  char command[99];
  memset(&command,'\0',99);

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
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_help(commandBuff,bufferSize);
  }
  else if (strcmp(command,"version") == 0) {
    //Run version
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_version();
  }
  else if (strcmp(command,"shutdown") == 0) {
    //Run shutdown
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    return run_shutdown(commandBuff, bufferSize);
  }
  else if (strcmp(command,"getdate") == 0) {
    //Run getdate
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_getdate();
  }
  else if (strcmp(command,"setdate") == 0) {
    //Run setdate
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_setdate(commandBuff);
  }
  else if (strcmp(command,"gettime") == 0) {
    //Run gettime
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_gettime();
  }
  else if (strcmp(command,"settime") == 0) {
    //Run settime
<<<<<<< HEAD
    run_settime(commandBuff);
=======
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_settime();
>>>>>>> 389c9c433a429531f7c6c1728b102caaedef3b9c
  }
  else if (strcmp(command,"clear") == 0) {
    //Run clear
    int clear_length = 5;
    //Clear code
    sys_req(WRITE,DEFAULT_DEVICE,"\e[2J",&clear_length);
    sys_req(WRITE,DEFAULT_DEVICE,"\e[2H",&clear_length);
  }
  else {
    //Invalid code
    char * errorMSG = "\n\x1b[1;31mInvalid command\x1b[1;0m\n";
    int errorMSG_length = strlen(errorMSG);
    //Print error message
    sys_req(WRITE,DEFAULT_DEVICE,errorMSG,&errorMSG_length);
  }

  return 0;
}
