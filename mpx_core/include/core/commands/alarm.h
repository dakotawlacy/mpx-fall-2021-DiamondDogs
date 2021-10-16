#ifndef _ALARM_H
#define _ALARM_H

/*
  Procedure..: run_getdate
  Description..: Runs the getdate function that reads the date from
  register values
*/
void alarm(char* buffer);

void add_alarm(char* hr, char* min, char* sec);

void check_alarm();

char* get_current_time();
#endif
