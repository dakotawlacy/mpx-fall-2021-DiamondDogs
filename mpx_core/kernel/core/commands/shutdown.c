#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/PCB.h>

int run_shutdown(char* buffer, int size) {

  //Print shutdown confirmation
  char* shutdown_check = "Are you sure you want to shutdown your system?\n";
  int sdCheck = strlen(shutdown_check);
  sys_req(WRITE, DEFAULT_DEVICE, shutdown_check, &sdCheck);

  //Clear buffer
  while(size >= 0){
    buffer[size] = '\0';
    size--;
  }
  size = 99;

  //Print choice message
  char* yn = "Type '1' to shutdown or Type '2' to continue working\n";
  int ynCheck = strlen(yn);
  sys_req(WRITE, DEFAULT_DEVICE, yn, &ynCheck);

  //Read in confirmation
  sys_req(READ, DEFAULT_DEVICE, buffer, &size);

  //Check what choice was make
  if(strcmp(buffer, "1") == 0){
    deleteAll();
    sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
  }

  if(strcmp(buffer, "2") == 0){
    //Run cancellation
    char * noSdMSG = "\nShutdown Denied\n";
    int noSdMSG_length = strlen(noSdMSG);
    sys_req(WRITE,DEFAULT_DEVICE,noSdMSG,&noSdMSG_length);
    return 0;
  }

  return 0;
}
