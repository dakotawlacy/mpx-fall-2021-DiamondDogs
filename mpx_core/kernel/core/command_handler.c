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
#include <core/PCB.h>
#include <core/commands/yield.h>
#include <core/loadr3.h>
#include <core/commands/alarm.h>

PCB* cop;

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

     sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

   }

  //Return when done
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
    if (run_shutdown(commandBuff, bufferSize)) {
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    }
  }
  else if (strcmp(command,"getdate") == 0) {
    //Run getdate
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_getdate();
  }
  else if (strcmp(command,"setdate") == 0) {
    //Run setdate
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    if (run_setdate(commandBuff)) {
      int invalidDate = 12;
      sys_req(WRITE,DEFAULT_DEVICE, "Invalid date\n", &invalidDate);
    }
  }
  else if (strcmp(command,"gettime") == 0) {
    //Run gettime
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    run_gettime();
  }
  else if (strcmp(command,"settime") == 0) {
    //Run settime
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    if (run_settime(commandBuff)) {
      int invalidTime = 12;
      sys_req(WRITE,DEFAULT_DEVICE, "Invalid time\n", &invalidTime);
    }
  }
  else if (strcmp(command,"clear") == 0) {
    //Run clear
    int clear_length = 5;
    //Clear code
    sys_req(WRITE,DEFAULT_DEVICE,"\e[2J",&clear_length);
    sys_req(WRITE,DEFAULT_DEVICE,"\e[2H",&clear_length);
  }
  ///////////////////////////
  else if(strcmp(command,"suspendPCB")==0){
    suspendPCB(commandBuff);
    char* temp = "PCB suspended\n";
    int temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  }
  else if(strcmp(command,"resumePCB")==0){
    resumePCB(commandBuff);
    char* temp = "PCB resumed\n";
    int temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  }
  else if(strcmp(command,"setPriority")==0){
    setPriority(commandBuff);
    char* temp = "PCB priority changed\n";
    int temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  }
  else if(strcmp(command,"showPCB")==0){
    showPCB(commandBuff);
  }
  else if(strcmp(command,"showAll")==0){
    printAll();
  }
  else if(strcmp(command,"showReady")==0){
    printReady();
  }
  else if(strcmp(command,"showBlocked")==0){
    printBlock();
  }
  /////////////////////////////
  // else if(strcmp(command,"createPCB")==0){
  //   int result = get_pcb_data(commandBuff);
  //
  //   if (result == 1) {
  //     char* temp = "PCB added\n";
  //     int temp_len = strlen(temp);
  //     sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //
  //   }
  //   else if (result == 1000) {
  //     //invalid name
  //     char* temp = "Invalid Name\n";
  //     int temp_len = strlen(temp);
  //     sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //   }
  //   else if (result == 2000) {
  //     //invalid class
  //     char* temp = "Invalid Class\n";
  //     int temp_len = strlen(temp);
  //     sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //   }
  //   else if (result == 3000) {
  //     //already inside
  //     char* temp = "Already Exists\n";
  //     int temp_len = strlen(temp);
  //     sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //   }
  //
  // }
  else if(strcmp(command,"deletePCB")==0){
    deletePCB(commandBuff);
  }

  // else if(strcmp(command,"blockPCB")==0){
  //   if(blockPCB(commandBuff) != NULL){
  //   char* temp = "PCB blocked\n";
  //   int temp_len = strlen(temp);
  //   sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //   }
  // }
  // else if(strcmp(command,"unblockPCB")==0){
  //   if(unblockPCB(commandBuff) != NULL){
  //   char* temp = "PCB unblocked\n";
  //   int temp_len = strlen(temp);
  //   sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  //   }
  // }
  // else if (strcmp(command,"yield") == 0) {
  //
  //   sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
  //   run_yield();
  // }
  else if (strcmp(command,"loadr3") == 0) {
    //Run gettime
    sys_req(WRITE,DEFAULT_DEVICE, "\n", &newLine);
    loadr3();
  }
  else if (strcmp(command,"alarm") == 0) {
    alarm(commandBuff);
  }
  else if (strcmp(command,"resumeAll") == 0) {
    resumeAll();
  }

  ///////////////////////////////////
  else {
    //Invalid code
    char * errorMSG = "\n\x1b[1;31mInvalid command\x1b[1;0m\n";
    int errorMSG_length = strlen(errorMSG);
    //Print error message
    sys_req(WRITE,DEFAULT_DEVICE,errorMSG,&errorMSG_length);
  }

  return 0;
}
