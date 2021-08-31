/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>

#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00); //disable interrupts
  outb(device + 3, 0x80); //set line control register
  outb(device + 0, 115200/9600); //set bsd least sig bit
  outb(device + 1, 0x00); //brd most significant bit
  outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
  (void)inb(device);      //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  outb(serial_port_out,'\r');
  outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  if (*msg == '\r') outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

int *polling(char *buffer, int *count){
// insert your code to gather keyboard input via the technique of polling.
// You must validat each key and handle special keys such as delete, back space, and
// arrow keys

//char* commandBuff = buffer;

//char bigBuffer[100];
 //memset(&bigBuffer,'\0',100);
 int place = 0;

while(1) {


  if (inb(COM1+5) & 1){
    char letter = inb(COM1);
    *buffer = letter;
    //place++;

    if (letter == 0x1B) {
      //serial_println("Escape");
      letter = inb(COM1);
      if (letter == 0x5B) {
        //serial_println("[");
        letter = inb(COM1);
        if (letter == 0x44){
         //serial_println(buffer);
           serial_println("left");
         }
        else if (letter == 0x43){
          //serial_println(buffer);
            serial_println("right");
       }
        else if (letter == 0x42){
        //serial_println(buffer);
          serial_println("down");
        }
        else  if (letter == 0x41){
         //serial_println(buffer);
           serial_println("up");
         }
       }
    }

    //If Escape is not the first key
    else{
    if (letter == (0x0B)){
      serial_println("backspace");
      //delete previous in buffer and screen
    }
    if (letter == (0x7F)){
      serial_println("delete");
      //delete key: delete next button
    }
    if (letter == (0x0A)){
      serial_println("");
      break;
      //Carriage Key
    }
    if (letter == (0x0D)){
      serial_println("");
      serial_println(buffer);
      break;
      //Carriage Key
    }

    if (letter >= (0x61) && letter <= (0x7A)){
      serial_print(&letter);
      //serial_println("lowercase");
      buffer[place] = letter;
      place++;
      //a-z
    }

      if (letter >= (0x41) && letter <= (0x5A)){
        serial_println("uppercase");
        //A-Z
      }
      //serial_println("number");
      //0-9
    //}
    if (letter >= (0x30) && letter <=(0x39)){
      //if (letter == (0x44)){
      serial_println("Number");
    }
  }
  //outb(COM1, buffer);
}

}
// remove the following line after implementing your module, this is present
// just to allow the program to compile before R1 is complete
strlen(buffer);

//commandBuff = buffer;

return count;
}
