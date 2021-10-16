#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/commands/alarm.h>
#include <core/struct.h>
#include <core/commands/gettime.h>
#include <core/commands/yield.h>

char* alarm1 = NULL;
char* alarm2 = NULL;
char* alarm3 = NULL;
char* alarm4 = NULL;
char* alarm5 = NULL;

char* getTime;
char* getHours;
char* getMin;
char* getSec;

void create_alarm() {
  //
  PCB* newPCB = setupPCB("alarm",2,1);
  context* cp = (context*) newPCB->stackTop;
  memset(cp,0,sizeof(context));
  cp->fs = 0x10;
  cp->gs = 0x10;
  cp->ds = 0x10;
  cp->es = 0x10;
  cp->cs = 0x8;
  cp->ebp = (u32int)(newPCB->stackBase);
  cp->esp = (u32int) (newPCB->stackTop);
  cp->eip = (u32int) &check_alarm;
  cp->eflags = 0x202;
  //
  removePCB(newPCB);


  newPCB->susState = 0;

  insertPCB(newPCB);

  alarm1 = sys_alloc_mem(sizeof(char[7]));
  alarm2 = sys_alloc_mem(sizeof(char[7]));
  alarm3 = sys_alloc_mem(sizeof(char[7]));
  alarm4 = sys_alloc_mem(sizeof(char[7]));
  alarm5 = sys_alloc_mem(sizeof(char[7]));

  getTime = sys_alloc_mem(sizeof(char[7]));
  getHours = sys_alloc_mem(sizeof(char[3]));
  getMin = sys_alloc_mem(sizeof(char[3]));
  getSec = sys_alloc_mem(sizeof(char[3]));


  memset(alarm1,'0',6);
  memset(alarm2,'0',6);
  memset(alarm3,'0',6);
  memset(alarm4,'0',6);
  memset(alarm5,'0',6);

  alarm1[6] = '\0';
  alarm2[6] = '\0';
  alarm3[6] = '\0';
  alarm4[6] = '\0';
  alarm5[6] = '\0';
}


void alarm(char* buffer){

  //Check to see if alarm PCB is already in queue
   if (findPCB("alarm") == NULL) {
    create_alarm();
   }

  char m[50];
  memset(m,'\0',50);

  //These will be used at a later time to use the time offered
  // int hours = 0;
  // int seconds = 0;
  // int minutes = 0;

  int i = 6;
  //Grabbing Message
  for (i = 6; i < strlen(buffer)-9; i++){
      m[i-6] = buffer[i];
  }

//  serial_println(m);

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
  //check_alarm();

  return;

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

  serial_println(hr);

  //serial_println("aaaaaaaa");

  if (strcmp(alarm1,"000000") == 0) {
    serial_println("alarm1");
    strcpy(alarm1,hr);
    return;
   }
  else if (strcmp(alarm2,"000000") == 0) {
    serial_println("alarm2");
    strcpy(alarm2,hr);
    return;
  }
  else if (strcmp(alarm3,"000000") == 0) {
    strcpy(alarm3,hr);
    return;
  }
  else if (strcmp(alarm4,"000000") == 0) {
    strcpy(alarm4,hr);
    return;
  }
  else if (strcmp(alarm5,"000000") == 0) {
    strcpy(alarm5,hr);
    return;
  } else {
    return;
  }


}

//PCB Func
void check_alarm() {

  while(1){
    // serial_println(alarm1);
    // serial_println(alarm2);
    // serial_println(alarm3);
    // serial_println(alarm4);
    // serial_println(alarm5);

    get_current_time();

    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }


  // serial_println(alarm4);
  // serial_println(alarm5);
  //sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

  //run_gettime();

}

char* get_current_time() {

  strcpy(getTime,"");

  // memset(getTime,'0',7);
  memset(getHours,'0',3);
  memset(getMin,'0',3);
  memset(getSec,'0',3);

  getTime[6] = '\0';
  getHours[2] = '\0';
  getMin[2] = '\0';
  getSec[2] = '\0';


  outb(0x70,0x04);
  int hourVal = inb(0x71);
  int hours = (hourVal & 0x0f) + ((hourVal/16)*10);

  itoa(hours, getHours);

  char* hr = "0";
  if (hours < 10) {
    strcat(hr,getHours);
  } else {
    strcpy(hr,getHours);
  }

  outb(0x70,0x02);
  int minutesVal = inb(0x71);
  int minutes = (minutesVal & 0x0f) + ((minutesVal/16)*10);

  itoa(minutes,getMin);

  char* min = "0";
  if (minutes < 10) {
    serial_println("FUCK");
    strcat(min,getMin);
  } else {
    strcpy(min,getMin);
  }

  outb(0x70,0x00);
  int secondsVal = inb(0x71);
  int seconds = (secondsVal & 0x0f) + ((secondsVal/16)*10);


  itoa(seconds,getSec);

  //serial_print(getHours);
  //serial_println(getMin);

  serial_println(hr);


   strcat(getTime,hr);
   strcat(getTime,min);
   //strcat(getTime,getSec);

   serial_println(getTime);




  return getTime;


}
