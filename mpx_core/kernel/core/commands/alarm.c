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

char* mess1;
char* mess2;
char* mess3;
char* mess4;
char* mess5;

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

  mess1 = sys_alloc_mem(sizeof(char[50]));
  mess2 = sys_alloc_mem(sizeof(char[50]));
  mess3 = sys_alloc_mem(sizeof(char[50]));
  mess4 = sys_alloc_mem(sizeof(char[50]));
  mess5 = sys_alloc_mem(sizeof(char[50]));

  mess1[49] = '\0';
  mess2[49] = '\0';
  mess3[49] = '\0';
  mess4[49] = '\0';
  mess5[49] = '\0';

  memset(alarm1,'Z',6);
  memset(alarm2,'Z',6);
  memset(alarm3,'Z',6);
  memset(alarm4,'Z',6);
  memset(alarm5,'Z',6);

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

  char message[50];
  memset(message,'\0',50);

  //These will be used at a later time to use the time offered
  int hours = 0;
  int seconds = 0;
  int minutes = 0;

  int i = 6;
  int hours, minutes, seconds = 0;
  //Grabbing Message
  for (i = 6; i < strlen(buffer)-9; i++){
      message[i-6] = buffer[i];
  }

  if(strlen(message) == 0){
<<<<<<< HEAD
   serial_println("Error: No Message Added");
   return;
  }

  if(buffer[strlen(buffer)-9] != ' '){
    serial_println("Error: Time Entered Incorrectly");
    return;
  }

  if(buffer[strlen(buffer)-8] == ':' || buffer[strlen(buffer)-7] == ':' ||
    buffer[strlen(buffer)-5] == ':'  || buffer[strlen(buffer)-4] == ':' ||
    buffer[strlen(buffer)-2] == ':'  || buffer[strlen(buffer)-1] == ':' ){
    serial_println("Error: Must Be Number in Hours, Minutes, or Seconds Slot");
    return;
  }

  if(buffer[strlen(buffer)-6] != ':'){
    serial_println("Error: Must Be Colon Between Numbers");
    return;
  }

  if(buffer[strlen(buffer)-3] != ':'){
    serial_println("Error: Must Be Colon Between Numbers");
    return;
  }

  if ((buffer[strlen(buffer)-1] < '0' || buffer[strlen(buffer)-1] > '9') || (buffer[strlen(buffer)-2] < '0' || buffer[strlen(buffer)-2] > '9')
  || (buffer[strlen(buffer)-4] < '0' || buffer[strlen(buffer)-4 ] > '9') || (buffer[strlen(buffer)-5] < '0' ||buffer[strlen(buffer)-5] > '9')
  || (buffer[strlen(buffer)-7] < '0' || buffer[strlen(buffer)- 7] > '9') || (buffer[strlen(buffer)-8] < '0' || buffer[strlen(buffer)-8] > '9')) {
    serial_println("Not a number");
    return;
  }
=======
    serial_println("Error: No Message Added");
    return;
  }

>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167

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

  if(buffer[strlen(buffer)-9] != ' '){
    serial_println("Error: Time Entered Incorrectly");
    return;
  }

  if(buffer[strlen(buffer)-8] == ':' || buffer[strlen(buffer)-7] == ':' ||
    buffer[strlen(buffer)-5] == ':'  || buffer[strlen(buffer)-4] == ':' ||
    buffer[strlen(buffer)-2] == ':'  || buffer[strlen(buffer)-1] == ':' ){
    serial_println("Error: Must Be Number in Hours, Minutes, or Seconds Slot");
    return;
  }

  if(buffer[strlen(buffer)-6] != ':'){
    serial_println("Error: Must Be Colon Between Numbers");
    return;
  }

  if(buffer[strlen(buffer)-3] != ':'){
    serial_println("Error: Must Be Colon Between Numbers");
    return;
  }

  times2[0] = buffer[strlen(buffer)-5];
  times2[1] = buffer[strlen(buffer)-4];
  times2[2] = '\0';

  times3[0] = buffer[strlen(buffer)-2];
  times3[1] = buffer[strlen(buffer)-1];
  times3[2] = '\0';

  char* copy = NULL;
  strcpy(copy,message);

  hours = atoi(times1);
  minutes = atoi(times2);
  seconds = atoi(times3);
<<<<<<< HEAD

  if (hours > 23){
    serial_print("Error: Hours too Large");
    return;
  }

  if (minutes > 59 || seconds > 59){
    serial_print("Error: Seconds or Minutes too large");
    return;
  }
=======
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167

  if (hours >= 24){
     serial_print("Error: Hours Either Too Large or Not Valid\n");
     return;
   }

   if (minutes > 60 || seconds > 60){
     serial_print("Error: Seconds or Minutes Either Too Large or Invalid\n");
     return;
  }

  add_alarm(times1,times2,times3, copy);


<<<<<<< HEAD

    return;

=======
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167
}

void add_alarm(char* hr, char* min, char* sec, char message[50]) {

  strcat(hr,min);
  strcat(hr,sec);

  char* add = "Alarm Added!\n";
  int add_len = strlen(add);
  char* full = "Alarms are full";
  int full_len = strlen(full);

  if (strcmp(alarm1,"ZZZZZZ") == 0) {
<<<<<<< HEAD
=======
    serial_println("\nAlarm1 has been added");
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167
    strcpy(alarm1,hr);
    strcpy(mess1,message);
    sys_req(WRITE, DEFAULT_DEVICE, add, &add_len);
    return;
   }
  else if (strcmp(alarm2,"ZZZZZZ") == 0) {
<<<<<<< HEAD
=======
    serial_println("\nAlarm2 has been added");
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167
    strcpy(alarm2,hr);
    strcpy(mess2,message);
    sys_req(WRITE, DEFAULT_DEVICE, add, &add_len);
    return;
  }
  else if (strcmp(alarm3,"ZZZZZZ") == 0) {
    serial_println("\nAlarm3 has been added");
    strcpy(alarm3,hr);
    strcpy(mess3,message);
    sys_req(WRITE, DEFAULT_DEVICE, add, &add_len);
    return;
  }
  else if (strcmp(alarm4,"ZZZZZZ") == 0) {
    serial_println("\nAlarm4 has been added");
    strcpy(alarm4,hr);
    strcpy(mess4,message);
    sys_req(WRITE, DEFAULT_DEVICE, add, &add_len);
    return;
  }
  else if (strcmp(alarm5,"ZZZZZZ") == 0) {
    serial_println("\nAlarm5 has been added");
    strcpy(alarm5,hr);
    strcpy(mess5,message);
    sys_req(WRITE, DEFAULT_DEVICE, add, &add_len);
    return;
  } else {
    sys_req(WRITE, DEFAULT_DEVICE, full, &full_len);
    return;
  }
}

//PCB Func
void check_alarm() {

  while(1){

    char* mes;
    int len;

    char* temp = get_current_time();
    if(strcmp(alarm1, temp) <= 0){
<<<<<<< HEAD

      mes = "Alarm 1: \n";
      len = strlen(mes);
      sys_req(WRITE, DEFAULT_DEVICE, mes, &len);
      len = strlen(mess1);
      sys_req(WRITE, DEFAULT_DEVICE, mess1, &len);
      serial_print("\n");
      memset(alarm1, 'Z', 6);
      memset(mess1, ' ', 49);
    }
    if(strcmp(alarm2, temp) <= 0){

      mes = "Alarm 2: \n";
      len = strlen(mes);
      sys_req(WRITE, DEFAULT_DEVICE, mes, &len);
      len = strlen(mess1);
      sys_req(WRITE, DEFAULT_DEVICE, mess2, &len);
      serial_print("\n");
      memset(alarm2, 'Z', 6);
      memset(mess2, ' ', 49);
    }
    if(strcmp(alarm3, temp) <= 0){

      mes = "Alarm 3: \n";
      len = strlen(mes);
      sys_req(WRITE, DEFAULT_DEVICE, mes, &len);
      len = strlen(mess1);
      sys_req(WRITE, DEFAULT_DEVICE, mess3, &len);
      serial_print("\n");
      memset(alarm2, 'Z', 6);
      memset(mess3, ' ', 49);
    }
    if(strcmp(alarm4, temp) <= 0){

      mes = "Alarm 4: \n";
      len = strlen(mes);
      sys_req(WRITE, DEFAULT_DEVICE, mes, &len);
      len = strlen(mess1);
      sys_req(WRITE, DEFAULT_DEVICE, mess4, &len);
      serial_print("\n");
=======
        serial_print("\nAlarm1: ");
        serial_print(mess1);
        serial_print(" has been dispatched\n");
        memset(alarm1, 'Z', 6);
        memset(mess1, ' ', 49);
    }
    if(strcmp(alarm2, temp) <= 0){
      serial_print("\nAlarm2: ");
      serial_print(mess2);
      serial_print(" has been dispatched\n");
        memset(alarm2, 'Z', 6);
        memset(mess2, ' ', 49);
    }
    if(strcmp(alarm3, temp) <= 0){
      serial_println("\nAlarm3: ");
      serial_print(mess3);
      serial_print("has been dispatched\n");
        memset(alarm2, 'Z', 6);
        memset(mess3, ' ', 49);
    }
    if(strcmp(alarm4, temp) <= 0){
      serial_println("\nAlarm4: ");
      serial_print(mess4);
      serial_print(" has been dispatched\n\n");
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167
      memset(alarm2, 'Z', 6);
      memset(mess4, ' ', 49);
    }
    if(strcmp(alarm5, temp) <= 0){
<<<<<<< HEAD

      mes = "Alarm 5: \n";
      len = strlen(mes);
      sys_req(WRITE, DEFAULT_DEVICE, mes, &len);
      len = strlen(mess1);
      sys_req(WRITE, DEFAULT_DEVICE, mess5, &len);
      serial_print("\n");
      memset(alarm2, 'Z', 6);
      memset(mess5, ' ', 49);
=======
      serial_println("\nAlarm5: ");
      serial_print(mess5);
      serial_print(" has been dispatched\n");
        memset(alarm2, 'Z', 6);
        memset(mess5, ' ', 49);
>>>>>>> 5b431d5fe83fbd12b2ac7fec956d12f7eff49167
    }

    if (strcmp(alarm1,"ZZZZZZ") == 0 && strcmp(alarm2,"ZZZZZZ")== 0 && strcmp(alarm3,"ZZZZZZ")== 0 && strcmp(alarm4,"ZZZZZZ")== 0 && strcmp(alarm5,"ZZZZZZ") == 0) {
       sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    } else {
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
  }


  // serial_println(alarm4);
  // serial_println(alarm5);
  //sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

  //run_gettime();

}

char* get_current_time() {

  memset(getTime, '\0', 7);
  memset(getHours, '\0', 3);
  memset(getMin, '\0', 3);
  memset(getSec, '\0', 3);

  //run_gettime();

  outb(0x70, 0x04);
  int hoursVal = inb(0x71);
  int hours = (hoursVal & 0x0f) + ((hoursVal/16)*10);

  outb(0x70, 0x02);
  int minutesVal = inb(0x71);
  int minutes = (minutesVal & 0x0f) + ((minutesVal/16)*10);

  outb(0x70, 0x00);
  int secondsVal = inb(0x71);
  int seconds = (secondsVal & 0x0f) + ((secondsVal/16)*10);

  itoa(hours, getHours);
  if(hours < 10){
    getHours[1] = getHours[0];
    getHours[0] = '0';
  }

  itoa(minutes, getMin);
  if(minutes < 10){
    getMin[1] = getMin[0];
    getMin[0] = '0';
  }

  itoa(seconds, getSec);
  if(seconds < 10){
    getSec[1] = getSec[0];
    getSec[0] = '0';
  }

  strcat(getTime, getHours);
  strcat(getTime, getMin);
  strcat(getTime, getSec);


  return getTime;
}
