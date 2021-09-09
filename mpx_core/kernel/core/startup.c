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

  //Logo lines
  char ln1[100] =  "\t                 HMMMMMMMMMMNNmd      .MMMMMMMMNNNmy                        \n";
  char ln2[100] =  "\t                 HhMMMMMMMMMMMMMNy.   .MMMMMMMMMMMMMMm+                     \n";
  char ln3[100] =  "\t                 HMMMMh   /sNMMMMm.   .MMMMM/--:+hMMMMMs                    \n";
  char ln4[100] =  "\t                 HMMMMs     .mMMMMy   .MMMMM.     +MMMMM-                   \n";
  char ln5[100] =  "\t                 HMMMMs      sMMMMm   .MMMMM.      NMMMM+                   \n";
  char ln6[100] =  "\t                 HMMMMs     `dMMMMh   .MMMMM.     -MMMMM:                   \n";
  char ln7[100] =  "\t                 HHMMMMy   ./hMMMMN   .MMMMM-   .oNMMMMd                    \n";
  char ln8[100] =  "\t                 HHMMMMNmmmMMMMMMm:   .MMMMMmmmNMMMMMMh`                    \n";
  char ln9[100] =  "\t                 HHMMMMMMMMMMMMd+`    .MMMMMMMMMMMMNy:                      \n";
  char ln10[100] = "\t                 H/ooooooooo/:`       `ooooooooo+/-       OS                  \n";

  //Length of lines
  int line_length = strlen(ln1);

  //Print logo
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
