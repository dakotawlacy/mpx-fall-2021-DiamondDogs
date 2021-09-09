#include <core/serial.h>
#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>


int run_startup() {

  //Run clear
  int clear_length = 5;
  //Clear code
  sys_req(WRITE,DEFAULT_DEVICE,"\e[2J",&clear_length);
  sys_req(WRITE,DEFAULT_DEVICE,"\e[2H",&clear_length);


  char ln1[100] =  "\t                   yMMMMMMMMNNmdo-     .MMMMMMMMNNNmy                     \n";
  char ln2[100] =  "\t                  hMMMMMMMMMMMMMNy.   .MMMMMMMMMMMMMMm+                   \n";
  char ln3[100] =  "\t                 HMMMMh   /sNMMMMm.  .MMMMM/--:+hMMMMMs                   \n";
  char ln4[100] =  "\t                 hMMMMs     .mMMMMy  .MMMMM.     +MMMMM-                  \n";
  char ln5[100] =  "\t                 hMMMMs      sMMMMm  .MMMMM.      NMMMM+                  \n";
  char ln6[100] =  "\t                 hMMMMs     `dMMMMh  .MMMMM.     -MMMMM:                  \n";
  char ln7[100] = "\t                  hMMMMy   ./hMMMMN:  .MMMMM-   .oNMMMMd                   \n";
  char ln8[100] = "\t                  hMMMMNmmmMMMMMMm:   .MMMMMmmmNMMMMMMh`                   \n";
  char ln9[100] = "\t                  hMMMMMMMMMMMMd+`    .MMMMMMMMMMMMNy:                     \n";
  char ln10[100] = "\t                  /ooooooooo/:`       `ooooooooo+/-                        \n";

  int line_length = strlen(ln1);

  sys_req(WRITE,DEFAULT_DEVICE,ln1,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln2,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln3,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln4,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln5,&line_length);

  sys_req(WRITE,DEFAULT_DEVICE,ln6,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln7,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln8,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln9,&line_length);
  sys_req(WRITE,DEFAULT_DEVICE,ln10,&line_length);

  return 0;
}
