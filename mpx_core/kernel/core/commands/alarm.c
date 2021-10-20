#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/commands/alarm.h>
#include <core/struct.h>
#include <core/commands/gettime.h>

char* alarm1 = NULL;
char* alarm2 = NULL;
char* alarm3 = NULL;
char* alarm4 = NULL;
char* alarm5 = NULL;

// void create_alarm() {
//
//   PCB* newPCB = setupPCB("alarm",1,1);
//   context* cp = (context*) newPCB->stackTop;
//   memset(cp,0,sizeof(context));
//   cp->fs = 0x10;
//   cp->gs = 0x10;
//   cp->ds = 0x10;
//   cp->es = 0x10;
//   cp->cs = 0x8;
//   cp->ebp = (u32int)(newPCB->stackBase);
//   cp->esp = (u32int) (newPCB->stackTop);
//   cp->eip = (u32int) &check_alarm;
//   cp->eflags = 0x202;
//
// }


void alarm (char* buffer){

  //Check to see if alarm PCB is already in queue
  // if (findPCB("alarm") == NULL) {
  //   create_alarm();
  // }

  char m [50];
  memset(m, '\0', 50);

  //These will be used at a later time to use the time offered
  // int hours = 0;
  // int seconds = 0;
  // int minutes = 0;

  int i = 6;

  //Grabbing Message
  for (i = 6; i < strlen(buffer)-9; i++){
      m[i-6] = buffer[i];
  }
  serial_println(m);


  //Capturing Hours:Minutes:Seconds
  char times1[3];
  char times2[3];
  char times3[3];

  memset(times1, '\0', 3);
  memset(times2, '\0', 3);
  memset(times3, '\0', 3);

  times1[0] = buffer[strlen(buffer)-8];
  times1[1] = buffer[strlen(buffer)-7];
  times1[2] = '\0';

  times2[0] = buffer[strlen(buffer)-5];
  times2[1] = buffer[strlen(buffer)-4];
  times2[2] = '\0';

  times3[0] = buffer[strlen(buffer)-2];
  times3[1] = buffer[strlen(buffer)-1];
  times3[2] = '\0';

  //serial_println(times1);
  //serial_println(times2);
  //serial_println(times3);

  add_alarm(times1,times2,times3);

  check_alarm();

  // hours = atoi(times1);
  // minutes = atoi(times2);
  // seconds = atoi(times3);

  // if (hours > 12){
  //   serial_print("Error: Hours too Large");
  //   return;
  // }
  //
  // if (minutes > 60 || seconds > 60){
  //   serial_print("Error: Seconds or Minutes too large");
  //   return;
  // }

}

void add_alarm(char* hr, char* min, char* sec) {

  strcat(hr,min);
  strcat(hr,sec);

  if (alarm1 == NULL) {
    alarm1 = hr;
    serial_println(alarm1);
  }
  else if (alarm2 == NULL) {
    alarm2 = hr;
    serial_println(alarm2);
  }
  else if (alarm3 == NULL) {
    alarm3 = hr;
    serial_println(alarm3);
  }
  else if (alarm4 == NULL) {
    alarm4 = hr;
    serial_println(alarm4);
  }
  else if (alarm5 == NULL) {
    alarm5 = hr;
    serial_println(alarm5);
  }
  else {
    serial_println("All alarm spots filled!");
  }
};

void check_alarm() {

  run_gettime();

}
