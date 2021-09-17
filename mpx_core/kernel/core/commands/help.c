#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_help(char * commandBuff, int bufferSize) {

  //Initialize command array
  char command[100];
  memset(&command, '\0', 99);
  //Copy first word into command
  int i = 5;
  for (i = 5; i < bufferSize; i++) {
    if (commandBuff[i] != ' ' || commandBuff[i] != '\0') {
      //Create temp variable
      char temp = commandBuff[i];
      //Place temp variable in to command array
      command[i - 5] = temp;
    }
  }

  if ((strcmp(command,"version") == 0) && strlen(command) == 7) {
    //Run help version
    char * versionFunc = "Type 'version' to see the current version of the OS.\n";
    int versionFunc_length = strlen(versionFunc);
    sys_req(WRITE,DEFAULT_DEVICE,versionFunc,&versionFunc_length);
  }
  else if ((strcmp(command,"shutdown") == 0) && strlen(command) == 8) {
    //Run help shutdown
    char * shutdownFunc = "Type 'shutdown' to terminate the process.\n";
    int shutdownFunc_length = strlen(shutdownFunc);
    sys_req(WRITE,DEFAULT_DEVICE,shutdownFunc,&shutdownFunc_length);
  }
  else if ((strcmp(command,"getdate") == 0) && strlen(command) == 7) {
    //Run help getdate
    char * getdateFunc = "Type 'getdate' to receive the current date.\n";
    int getdateFunc_length = strlen(getdateFunc);
    sys_req(WRITE,DEFAULT_DEVICE,getdateFunc,&getdateFunc_length);
  }
  else if ((strcmp(command,"setdate") == 0) && strlen(command) == 7) {
    //Run help setdate
    char * setdateFunc = "Type 'setdate MM DD YYYY' to set the date to whatever date is current.\n";
    int setdateFunc_length = strlen(setdateFunc);
    sys_req(WRITE,DEFAULT_DEVICE,setdateFunc,&setdateFunc_length);
  }
  else if ((strcmp(command,"gettime") == 0) && strlen(command) == 7) {
    //Run  help gettime
    char * gettimeFunc = "Type 'gettime' to receive the current time.\n";
    int gettimeFunc_length = strlen(gettimeFunc);
    sys_req(WRITE,DEFAULT_DEVICE,gettimeFunc,&gettimeFunc_length);
  }
  else if ((strcmp(command,"settime") == 0) && strlen(command) == 7) {
    //Run help settime
    char * settimeFunc = "Type 'settime HH MM SS' to set the time to whatever time is current.\n";
    int settimeFunc_length = strlen(settimeFunc);
    sys_req(WRITE,DEFAULT_DEVICE,settimeFunc,&settimeFunc_length);

  }
  else if ((strcmp(command,"clear") == 0) && strlen(command) == 5) {
    //Run help clear
    char * clearFunc = "Type 'clear' to clear screen\n";
    int clearFunc_length = strlen(clearFunc);
    sys_req(WRITE,DEFAULT_DEVICE,clearFunc,&clearFunc_length);
  }
  ///////////////////////////////

  //WRITE SYNTAX FOR NEW COMMANDS IN HERE...

  ///////////////////////////////
  else {
    //Invalid command catch
    if(strlen(commandBuff) > 4){
      char * error = "\x1b[1;31mInvalid command\x1b[1;0m\n";
      int errorLength = strlen(commandBuff);
      sys_req(WRITE,DEFAULT_DEVICE,error, &errorLength);
  }
  else {

      //Initialize char arrays and lengths
      char * helpList = "\nList of Commands:\n";
      int helpList_length = strlen(helpList);

      char * versionHelp = "'version' - To receive the OS current version.\n";
      int versionHelp_length = strlen(versionHelp);

      char * getdateHelp = "'getdate' - To get the current date.\n";
      int getdateHelp_length = strlen(getdateHelp);

      char * gettimeHelp = "'gettime' - To get the current time.\n";
      int gettimeHelp_length = strlen(gettimeHelp);

      char * settimeHelp = "'settime' - To set the current time.\n";
      int settimeHelp_length = strlen(settimeHelp);

      char * setdateHelp = "'setdate' - To set the current date.\n";
      int setdateHelp_length = strlen(setdateHelp);

      char * shutdownHelp = "'shutdown' - To shutdown the OS.\n";
      int shutdownHelp_length = strlen(shutdownHelp);

      char * clearHelp = "'clear' - To clear the terminal window.\n";
      int clearHelp_length = strlen(clearHelp);
////////////////////////
      char * suspendHelp = "'suspend' - to suspend.....\n";
      int suspendHelp_length = strlen(suspendHelp);

      char * resumeHelp = "'resume' - to resume.....\n";
      int resumeHelp_length = strlen(resumeHelp);

      char * setPrioHelp = "'set_priority' - to set priority.....\n";
      int setPrioHelp_length = strlen(setPrioHelp);

      char * showPCBHelp = "'show_pcb' - to show the PCB.....\n";
      int showPCBHelp_length = strlen(showPCBHelp);

      char * showAllHelp = "'show_all' - to show all.....\n";
      int showAllHelp_length = strlen(showAllHelp);

      char * showReadyHelp = "'show_ready' - to show ready processes.....\n";
      int showReadyHelp_length = strlen(showReadyHelp);

      char * showBlockedHelp = "'show_blocked' - to show blocked processes.....\n";
      int showBlockedHelp_length = strlen(showBlockedHelp);

      char * createPCBHelp = "'create_pcb' - to create the pcb.....\n";
      int createPCBHelp_length = strlen(createPCBHelp);

      char * deletePCBHelp = "'delete_pcb' - to delete the pcb.....\n";
      int deletePCBHelp_length = strlen(deletePCBHelp);

      char * blockHelp = "'block' - to block a process.....\n";
      int blockHelp_length = strlen(blockHelp);

      char * unblockHelp = "'unblock' - to unblock a process.....\n";
      int unblockHelp_length = strlen(unblockHelp);
///////////////////////////
      char * helpFunc = "\nPlease use <help 'commandname'> to view commands functionality.\n";
      int helpFunc_length = strlen(helpFunc);

      //List commands
      sys_req(WRITE,DEFAULT_DEVICE,helpList,&helpList_length);
      sys_req(WRITE,DEFAULT_DEVICE,versionHelp,&versionHelp_length );
      sys_req(WRITE,DEFAULT_DEVICE,getdateHelp,&getdateHelp_length );
      sys_req(WRITE,DEFAULT_DEVICE,gettimeHelp,&gettimeHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE,settimeHelp,&settimeHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE,setdateHelp,&setdateHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE,shutdownHelp,&shutdownHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, clearHelp, &clearHelp_length);
      /////////////////////////////
      sys_req(WRITE,DEFAULT_DEVICE, suspendHelp, &suspendHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, resumeHelp, &resumeHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, setPrioHelp, &setPrioHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, showPCBHelp, &showPCBHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, showAllHelp, &showAllHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, showReadyHelp, &showReadyHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, showBlockedHelp, &showBlockedHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, createPCBHelp, &createPCBHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, deletePCBHelp, &deletePCBHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, blockHelp, &blockHelp_length);
      sys_req(WRITE,DEFAULT_DEVICE, unblockHelp, &unblockHelp_length);

      ////////////////////////////
      sys_req(WRITE,DEFAULT_DEVICE,helpFunc,&helpFunc_length);
    }
  }

  //Clear buffer
  int buffClear = 99;
  while(buffClear >= 0){
    command[buffClear] = '\0';
    buffClear--;
  }
   buffClear = 99;

  return 0;
}
