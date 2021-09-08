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

char ln1[100] =  "\t               .yNdddddddddddddddddddddddddddddddddddddddNy.               \n";
char ln2[100] =  "\t             .ymo                                         omy.             \n";
char ln3[100] =  "\t           .smo                                             oms.           \n";
char ln4[100] =  "\t        .sds.      yMMMMMMMMNNmdo-     .MMMMMMMMNNNmy         .ods.        \n";
char ln5[100] =  "\t     .ods.        hMMMMMMMMMMMMMNy.   .MMMMMMMMMMMMMMm+          .sdo.     \n";
char ln6[100] =  "\t   .ods.          HMMMMh   /sNMMMMm.  .MMMMM/--:+hMMMMMs           .sdo.   \n";
char ln7[100] =  "\t .ody-            hMMMMs     .mMMMMy  .MMMMM.     +MMMMM-            -ydo. \n";
char ln8[100] =  "\t/md-              hMMMMs      sMMMMm  .MMMMM.      NMMMM+              -dm/\n";
char ln9[100] =  "\t.sdo.             hMMMMs     `dMMMMh  .MMMMM.     -MMMMM:             .ods.\n";
char ln10[100] = "\t  .smo            hMMMMy   ./hMMMMN:  .MMMMM-   .oNMMMMd             oms.  \n";
char ln11[100] = "\t    .ymo          hMMMMNmmmMMMMMMm:   .MMMMMmmmNMMMMMMh`           omy.    \n";
char ln12[100] = "\t      .ym+        hMMMMMMMMMMMMd+`    .MMMMMMMMMMMMNy:           +my.      \n";
char ln13[100] = "\t        -ym+      /ooooooooo/:`       `ooooooooo+/-            +my-        \n";
char ln14[100] = "\t          -ym+                  .--  .    .--.               +my-          \n";
char ln15[100] = "\t            -hm/              /s   -h  ss   /              +my-            \n";
char ln16[100] = "\t              -hm/            M:    ds -yy+.             /mh-              \n";
char ln17[100] = "\t                :hd/          N:    do   ./ho          /dh:                \n";
char ln18[100] = "\t                  :hd/        :o---:s  /.  /y        /dh:                  \n";
char ln19[100] = "\t                   `:hh:       -----    ---/       :hh:                    \n";
char ln20[100] = "\t                      /hh:                       :hh/                      \n";
char ln21[100] = "\t                        /dh:                   :hd/                        \n";
char ln22[100] = "\t                          /dy-               -hd/                          \n";
char ln23[100] = "\t                            +dy-           -yd+                            \n";
char ln24[100] = "\t                              +my-       -ym+                              \n";
char ln25[100] = "\t                                +my.   .ym+                                \n";
char ln26[100] = "\t                                  omy-ymo                                  \n";
char ln27[100] = "\t                                   `omo                                    \n";

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

sys_req(WRITE,DEFAULT_DEVICE,ln11,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln12,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln13,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln14,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln15,&line_length);

sys_req(WRITE,DEFAULT_DEVICE,ln16,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln17,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln18,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln19,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln20,&line_length);

sys_req(WRITE,DEFAULT_DEVICE,ln21,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln22,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln23,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln24,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln25,&line_length);

sys_req(WRITE,DEFAULT_DEVICE,ln26,&line_length);
sys_req(WRITE,DEFAULT_DEVICE,ln27,&line_length);



  return 0;
}
