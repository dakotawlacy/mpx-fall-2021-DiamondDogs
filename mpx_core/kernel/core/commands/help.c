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

  //char* print = sys_alloc_mem(99);
  //serial_print(command);

  if ((strcmp(command,"version") == 0) && strlen(command) == 7) {
    //Run help version
    char* versionFunc = "Type 'version' to see the current version of the OS.\n";
    int versionFunc_length = strlen(versionFunc);
     sys_req(WRITE,DEFAULT_DEVICE,versionFunc,&versionFunc_length);
  }
  // else if ((strcmp(command,"shutdown") == 0) && strlen(command) == 8) {
  //   //Run help shutdown
  //   char * shutdownFunc = "Type 'shutdown' to terminate the process.\n";
  //   int shutdownFunc_length = strlen(shutdownFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,shutdownFunc,&shutdownFunc_length);
  // }
  // else if ((strcmp(command,"getdate") == 0) && strlen(command) == 7) {
  //   //Run help getdate
  //   char * getdateFunc = "Type 'getdate' to receive the current date.\n";
  //   int getdateFunc_length = strlen(getdateFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,getdateFunc,&getdateFunc_length);
  // }
  // else if ((strcmp(command,"setdate") == 0) && strlen(command) == 7) {
  //   //Run help setdate
  //   char * setdateFunc = "Type 'setdate MM DD YYYY' to set the date to whatever date is current.\n";
  //   int setdateFunc_length = strlen(setdateFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,setdateFunc,&setdateFunc_length);
  // }
  // else if ((strcmp(command,"gettime") == 0) && strlen(command) == 7) {
  //   //Run  help gettime
  //   char * gettimeFunc = "Type 'gettime' to receive the current time.\n";
  //   int gettimeFunc_length = strlen(gettimeFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,gettimeFunc,&gettimeFunc_length);
  // }
  // else if ((strcmp(command,"settime") == 0) && strlen(command) == 7) {
  //   //Run help settime
  //   char * settimeFunc = "Type 'settime HH MM SS' to set the time to whatever time is current.\n";
  //   int settimeFunc_length = strlen(settimeFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,settimeFunc,&settimeFunc_length);
  //
  // }
  // else if ((strcmp(command,"clear") == 0) && strlen(command) == 5) {
  //   //Run help clear
  //   char * clearFunc = "Type 'clear' to clear screen\n";
  //   int clearFunc_length = strlen(clearFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,clearFunc,&clearFunc_length);
  // }
  // else if ((strcmp(command,"suspendPCB") == 0) && strlen(command) == 10) {
  //   //Run help clear
  //   char * suspendFunc = "Type 'suspendPCB NameOfPCB' to suspend that PCB\n";
  //   int suspendFunc_length = strlen(suspendFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,suspendFunc,&suspendFunc_length);
  // }
  // else if ((strcmp(command,"resumePCB") == 0) && strlen(command) == 9) {
  //   //Run help clear
  //   char * resumeFunc = "Type 'resumePCB NameOfPCB' to resume that PCB\n";
  //   int resumeFunc_length = strlen(resumeFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,resumeFunc,&resumeFunc_length);
  // }
  // else if ((strcmp(command,"setPriority") == 0) && strlen(command) == 11) {
  //   //Run help clear
  //   char * setPrioFunc = "Type 'setPriority <NameOfPCB> <priority value between 0 and 9>' to change the priority of that PCB\n";
  //   int setPrioFunc_length = strlen(setPrioFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,setPrioFunc,&setPrioFunc_length);
  // }
  // else if ((strcmp(command,"showPCB") == 0) && strlen(command) == 7) {
  //   //Run help clear
  //   char * showPCBFunc = "Type 'showPCB <NameOfPCB>' to show the contents of the PCB\n";
  //   int showPCBFunc_length = strlen(showPCBFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,showPCBFunc,&showPCBFunc_length);
  // }
  // else if ((strcmp(command,"showAll") == 0) && strlen(command) == 7) {
  //   //Run help clear
  //   char * showAllFunc = "Type 'showAll' to show PCB's from ready and blocked queue\n";
  //   int showAllFunc_length = strlen(showAllFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,showAllFunc,&showAllFunc_length);
  // }
  // else if ((strcmp(command,"showReady") == 0) && strlen(command) == 9) {
  //   //Run help clear
  //   char * showReadyFunc = "Type 'showReady' to show PCB's from ready and suspended ready queue\n";
  //   int showReadyFunc_length = strlen(showReadyFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,showReadyFunc,&showReadyFunc_length);
  // }
  // else if ((strcmp(command,"showBlocked") == 0) && strlen(command) == 11) {
  //   //Run help clear
  //   char * showBlockedFunc = "Type 'clear' to clear screen\n";
  //   int showBlockedFunc_length = strlen(showBlockedFunc);
  //   sys_req(WRITE,DEFAULT_DEVICE,showBlockedFunc,&showBlockedFunc_length);
  // }
  // ///////////////////////////////
  // //WRITE SYNTAX FOR NEW COMMANDS IN HERE...
  // else if ((strcmp(command,"loadr3") == 0) && strlen(command) == 6) {
  //   //Run help clear
  //   char * showLoadR3 = "Type 'loadr3' to load R3 commands\n";
  //   int showLoadR3_length = strlen(showLoadR3);
  //   sys_req(WRITE,DEFAULT_DEVICE,showLoadR3,&showLoadR3_length);
  // }
  // else if ((strcmp(command,"alarm") == 0) && strlen(command) == 5) {
  //   //Run help clear
  //   char * showAlarm = "Type 'alarm <message> hr:min:sec' to add an alarm. 5 alarm slots are available\n";
  //   int showAlarm_length = strlen(showAlarm);
  //   sys_req(WRITE,DEFAULT_DEVICE,showAlarm,&showAlarm_length);
  // }
  // else if ((strcmp(command,"resumeAll") == 0) && strlen(command) == 9) {
  //   //Run help clear
  //   char * showResumeAll = "Type 'resumeAll' to resume all PCB's in the suspended ready queue\n";
  //   int showResumeAll_length = strlen(showResumeAll);
  //   sys_req(WRITE,DEFAULT_DEVICE,showResumeAll,&showResumeAll_length);
  // }
  // else if ((strcmp(command,"infinite") == 0) && strlen(command) == 8) {
  //   //Run help clear
  //   char * showInfinite = "Type 'infinite' to create an infinite process\n";
  //   int showInfinite_length = strlen(showInfinite);
  //   sys_req(WRITE,DEFAULT_DEVICE,showInfinite,&showInfinite_length);
  // }
  // else if ((strcmp(command,"showFree") == 0) && strlen(command) == 8) {
  //   char* showFree = "Type 'showFree' to show free MCB's\n";
  //   int showFree_len = strlen(showFree);
  //   sys_req(WRITE,DEFAULT_DEVICE,showFree,&showFree_len);
  // }
  // else if ((strcmp(command,"showAlloc") == 0) && strlen(command) == 9) {
  //   char* showAlloc = "Type 'showAlloc' to show allocated MCB's\n";
  //   int showAlloc_len = strlen(showAlloc);
  //   sys_req(WRITE,DEFAULT_DEVICE,showAlloc,&showAlloc_len);
  // }
  // ///////////////////////////////
  // else {
  //   //Invalid command catch
  //   if(strlen(commandBuff) > 4){
  //     char* error = "Invalid command\n";
  //     int errorLength = strlen(commandBuff);
  //     sys_req(WRITE,DEFAULT_DEVICE,error, &errorLength);
  // }
  else {

      //Initialize char arrays and lengths
      //char * helpList = "List of Commands:";
      //int helpList_length = strlen(helpList);
      //char* print = "List of Commands:\n";
      //int print_len = strlen(print);

      char * versionHelp = "'version' - To receive the OS current version.\n";
      int versionHelp_length = strlen(versionHelp);

      // char * getdateHelp = "'getdate' - To get the current date.\n";
      // int getdateHelp_length = strlen(getdateHelp);
      //
      // char * gettimeHelp = "'gettime' - To get the current time.\n";
      // int gettimeHelp_length = strlen(gettimeHelp);
      //
      // char * settimeHelp = "'settime' - To set the current time.\n";
      // int settimeHelp_length = strlen(settimeHelp);
      //
      // char * setdateHelp = "'setdate' - To set the current date.\n";
      // int setdateHelp_length = strlen(setdateHelp);
      //
      // char * shutdownHelp = "'shutdown' - To shutdown the OS.\n";
      // int shutdownHelp_length = strlen(shutdownHelp);
      //
      // char * clearHelp = "'clear' - To clear the terminal window.\n";
      // int clearHelp_length = strlen(clearHelp);
      //
      // char * suspendHelp = "'suspendPCB' - to suspend selected PCB\n";
      // int suspendHelp_length = strlen(suspendHelp);
      //
      // char * resumeHelp = "'resumePCB' - to resume selected PCB\n";
      // int resumeHelp_length = strlen(resumeHelp);
      //
      // char * setPrioHelp = "'setPriority' - to set new priority of selected PCB\n";
      // int setPrioHelp_length = strlen(setPrioHelp);
      //
      // char * showPCBHelp = "'showPCB' - to show the contents of selected PCB\n";
      // int showPCBHelp_length = strlen(showPCBHelp);
      //
      // char * showAllHelp = "'showAll' - to show all PCB's in a queue\n";
      // int showAllHelp_length = strlen(showAllHelp);
      //
      // char * showReadyHelp = "'showReady' - to show ready processes\n";
      // int showReadyHelp_length = strlen(showReadyHelp);
      //
      // char * showBlockedHelp = "'showBlocked' - to show blocked processes\n";
      // int showBlockedHelp_length = strlen(showBlockedHelp);
      //
      // char * showLoadR3 = "'loadr3' - to show load R3 processes\n";
      // int showLoadR3_length = strlen(showLoadR3);
      //
      // char * showAlarm = "'alarm' - to create an alarm\n";
      // int showAlarm_length = strlen(showAlarm);
      //
      // char * showResumeAll = "'resumeAll' - to resume all processes\n";
      // int showResume_length = strlen(showResumeAll);
      //
      // char * showInfinite = "'infinite' - to create an infinite process\n";
      // int showInfinite_length = strlen(showInfinite);
      //
      // char* showFree = "'showFree' - to show Free MCB's\n";
      // int showFree_len = strlen(showFree);
      //
      // char* showAlloc = "'showAlloc' - to show Allocated MCB's\n";
      // int showAlloc_len = strlen(showAlloc);
      //
      // char * helpFunc = "\nPlease use <help 'commandname'> to view commands functionality.\n";
      // int helpFunc_length = strlen(helpFunc);

      //List commands
      //sys_req(WRITE,DEFAULT_DEVICE,print,&print_len);
      sys_req(WRITE,DEFAULT_DEVICE,versionHelp,&versionHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE,getdateHelp,&getdateHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE,gettimeHelp,&gettimeHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE,settimeHelp,&settimeHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE,setdateHelp,&setdateHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE,shutdownHelp,&shutdownHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, clearHelp, &clearHelp_length);
      // /////////////////////////////
      // sys_req(WRITE,DEFAULT_DEVICE, suspendHelp, &suspendHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, resumeHelp, &resumeHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, setPrioHelp, &setPrioHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showPCBHelp, &showPCBHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showAllHelp, &showAllHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showReadyHelp, &showReadyHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showBlockedHelp, &showBlockedHelp_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showLoadR3, &showLoadR3_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showAlarm, &showAlarm_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showResumeAll, &showResume_length);
      // sys_req(WRITE,DEFAULT_DEVICE, showInfinite, &showInfinite_length);
      // sys_req(WRITE,DEFAULT_DEVICE,showFree,&showFree_len);
      // sys_req(WRITE,DEFAULT_DEVICE,showAlloc,&showAlloc_len);
      ////////////////////////////
      //sys_req(WRITE,DEFAULT_DEVICE,helpFunc,&helpFunc_length);
    //}
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
