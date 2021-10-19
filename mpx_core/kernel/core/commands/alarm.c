#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <core/commands/alarm.h>

void alarm (char* buffer){
  char m [50];
  memset(m, '\0', 50);

  //These will be used at a later time to use the time offered
  int hours = 0;
  int seconds = 0;
  int minutes = 0;




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


  serial_println(times1);
  serial_println(times2);
  serial_println(times3);

  hours = atoi(times1);
  minutes = atoi(times2);
  seconds = atoi(times3);

  if (hours > 24){
    serial_print("Error: Hours too Large");
    return;
  }

  if (minutes > 60 || seconds > 60){
    serial_print("Error: Seconds or Minutes too large");
    return;
  }

  }
