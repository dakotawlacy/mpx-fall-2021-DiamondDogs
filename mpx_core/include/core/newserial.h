#ifndef _NEWSERIAL_H
#define _NEWSERIAL_H


void set_int(int bit, int on);

void top_handle();

int com_open(int baud_rate);

int com_write(char* buffer, int* count);

int com_read();

void write_interrupt();

#endif
