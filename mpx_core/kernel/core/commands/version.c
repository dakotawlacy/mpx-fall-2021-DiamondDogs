#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

int run_version() {

  //Print version
  char * versionDisp = "DiamondDogs OS Version R5.\n";
  int versionDisp_length = strlen(versionDisp);
  sys_req(WRITE,DEFAULT_DEVICE,versionDisp,&versionDisp_length);

  return 0;

}
