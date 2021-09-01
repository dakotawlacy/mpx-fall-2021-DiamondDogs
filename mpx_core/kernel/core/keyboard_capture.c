#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/keyboard_capture.h>
#include <core/command_handler.h>
#include <core/serial.h>

int keyCap(char* buffer, int location, char letter){
    if (letter == 0x1B) {
      //serial_println("Escape");
      letter = inb(COM1);
      if (letter == 0x5B) {
        //serial_println("[");
        letter = inb(COM1);
        //serial_println(buffer);
        if (letter == 0x44){
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
      //serial_println("");
      return 5;
      //Carriage Key
    }
    if (letter == (0x0D)){
      //serial_println("");
      //serial_println(buffer);
      return 5;
      //Carriage Key
    }

    if (letter >= (0x61) && letter <= (0x7A)){
      serial_print(&buffer[location]);
      //serial_println("lowercase");
      buffer[location] = letter;
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

return 0;
}
