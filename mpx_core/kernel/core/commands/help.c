#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_help() {

  char * helpList = "List of Commands:\n";
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

  char * helpFunc = "Please use <help 'commandname'> to view commands functionality.\n";
  int helpFunc_length = strlen(helpFunc);

  sys_req(WRITE,DEFAULT_DEVICE,helpList,&helpList_length);
  sys_req(WRITE,DEFAULT_DEVICE,versionHelp,&versionHelp_length );
  sys_req(WRITE,DEFAULT_DEVICE,getdateHelp,&getdateHelp_length );
  sys_req(WRITE,DEFAULT_DEVICE,gettimeHelp,&gettimeHelp_length);
  sys_req(WRITE,DEFAULT_DEVICE,settimeHelp,&settimeHelp_length);
  sys_req(WRITE,DEFAULT_DEVICE,setdateHelp,&setdateHelp_length);
  sys_req(WRITE,DEFAULT_DEVICE,shutdownHelp,&shutdownHelp_length);
  sys_req(WRITE,DEFAULT_DEVICE,helpFunc,&helpFunc_length);

//  int i = 4;
//  serial_println(commandBuff);
//  char command[99];
  //Copy first word into command
//  for(i = 4; i < strlen(commandBuff); i++) {
//    if (commandBuff[i] == ' ' || commandBuff[i] == '\0') {
//      break;
//    } else {
//      char temp= commandBuff[i];

//      command[i] = temp;
//    }
//  }

//  if (strcmp(command,"version") == 0) {
    //Run version
    char * versionFunc = "Type 'version' to see the current version of the OS.\n";
    int versionFunc_length = strlen(versionFunc);
    sys_req(WRITE,DEFAULT_DEVICE,versionFunc,&versionFunc_length);
//    run_version();
//  }

//  if (strcmp(command,"shutdown") == 0) {
    //Run shutdown
    char * shutdownFunc = "Type 'shutdown' to terminate the process.\n";
    int shutdownFunc_length = strlen(shutdownFunc);
    sys_req(WRITE,DEFAULT_DEVICE,shutdownFunc,&shutdownFunc_length);
//  run_shutdown();
//  }

//  if (strcmp(command,"getdate") == 0) {
    //Run getdate
    char * getdateFunc = "Type 'getdate' to receive the current date.\n";
    int getdateFunc_length = strlen(getdateFunc);
    sys_req(WRITE,DEFAULT_DEVICE,getdateFunc,&getdateFunc_length);
//  run_getdate();
//  }

//  if (strcmp(command,"setdate") == 0) {
    //Run setdate
    char * setdateFunc = "Type 'setdate MM/DD/YYYY' to set the date to whatever date is current.\n";
    int setdateFunc_length = strlen(setdateFunc);
    sys_req(WRITE,DEFAULT_DEVICE,setdateFunc,&setdateFunc_length);

//  run_setdate();
//  }

//  if (strcmp(command,"gettime") == 0) {
    //Run gettime
    char * gettimeFunc = "Type 'gettime' to receive the current time.\n";
    int gettimeFunc_length = strlen(gettimeFunc);
    sys_req(WRITE,DEFAULT_DEVICE,gettimeFunc,&gettimeFunc_length);
//  run_gettime();
//  }

//  if (strcmp(command,"settime") == 0) {
    //Run settime
    char * settimeFunc = "Type 'settime HH:MM' to set the time to whatever time is current.\n";
    int settimeFunc_length = strlen(settimeFunc);
    sys_req(WRITE,DEFAULT_DEVICE,settimeFunc,&settimeFunc_length);
//  run_settime();
//  }


  return 0;
}
