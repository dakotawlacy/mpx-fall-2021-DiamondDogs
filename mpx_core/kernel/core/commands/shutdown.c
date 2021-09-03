#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_shutdown(char* buffer, int size) {
  char* shutdown_check = "Are you sure you want to shutdown your system?\n";
  int sdCheck = strlen(shutdown_check);

  sys_req(WRITE, DEFAULT_DEVICE, shutdown_check, &sdCheck);

  while(size >= 0){
    buffer[size] = '\0';
    size--;
  }
  size = 99;

  char* yn = "1. Yes \n2. No\n";
  int ynCheck = strlen(yn);

  sys_req(WRITE, DEFAULT_DEVICE, yn, &ynCheck);

  sys_req(READ, DEFAULT_DEVICE, buffer, &size);

  if(strcmp(buffer, "1") == 0){
    //sys_req(EXIT, DEFAULT_DEVICE, "\0", 0);
    return 1;
  }

  return 0;
}
