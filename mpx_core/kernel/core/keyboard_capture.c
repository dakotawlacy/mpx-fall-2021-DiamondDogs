#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/keyboard_capture.h>
#include <core/command_handler.h>
#include <core/serial.h>

int keyCap(char* buffer, int location, int length) {

  //Get letter and length
  char letter = inb(COM1);
  length = strlen(buffer);

    //Check to see if ESC is pressed first
    if (letter == 0x1B) {
      //Get next char
      letter = inb(COM1);
      //Check to see if [ is next
      if (letter == 0x5B) {
        //Get next char
        letter = inb(COM1);
        if (letter == '3') {
          letter = inb(COM1);
          if (letter == '~') {
            //Check end
            if (location == length) {

              return location;
            }
            //If not end
            else {
              //May be able to in future change for loop to saved cursor position
              int i = location;
              for (i = location; i < length; i++) {
              buffer[i] = buffer[i + 1];

              }
              //Reset to beginning of line
              serial_print("\e[0F");
              serial_print("\e[0E");

              //Print buffer
              serial_print(&buffer[0]);
              //Clear last character
              serial_print(" ");

              //Go back to beginning of line
              serial_print("\e[0F");
              serial_print("\e[0E");

              //Set cursor back to originial position
              int j = 0;
              for (j = 0; j < location; j++) {
                serial_print("\e[1C");
              }

              //Decrement length
              length--;

              return location;
            }
            //In middle
          }
        }
        //Check to see what arrow key is pressed
        //If left
        if (letter == 0x44) {
          //If at the beginning
          if (location == 0) {
            //Do nothing
            return location;
          }
          else {
            //Move Location left one in logic
            location = location - 1;
            //Move location on screen left
            serial_print("\e[1D");
            //Reset buffer
            letter = inb(COM1);

            return location;
          }
        }
        //If right
        else if (letter == 0x43) {
          //If at end
          if (location == strlen(buffer)) {
            //Do nothing
            return location;
          }
          else {
            //Increase location by 1
            location = location + 1;
            //Move location on screen right
            serial_print("\e[1C");
            //Reset buffer
            letter = inb(COM1);

            return location;

          }
        }
        else if (letter == 0x42){
          serial_println("down");///Down
        }
        else  if (letter == 0x41){
          serial_println("up");//Up
        }
      }

    }//End ESC check

    //If Escape is not the first key
    else {

      //Backspace - NEED TO CHECK IF AT END, BEGINNING OR MIDDLE
      if (letter == (0x7F)) {
        //Check if at beginning
        if (location == 0) {
          //do nothing
          return location;
        }//Check if end
        else if (location == strlen(buffer)) {
          //Move left in location
          --location;
          --length;

          //Visually move left
          serial_print("\e[1D");

          //Set value to null pointer
          buffer[location] = '\0';

          //Clear letter
          serial_print(" ");

          //Visually Move left
          serial_print("\e[1D");

          return location;

        }//In the middle
        else {

          --location;

          int i = location;
          for (i = location; i < length; i++) {
          buffer[i] = buffer[i + 1];

          }

          //Reset to beginning of line
          serial_print("\e[0F");
          serial_print("\e[0E");

          //Print buffer
          serial_print(&buffer[0]);
          //Clear last character
          serial_print(" ");

          //Go back to beginning of line
          serial_print("\e[0F");
          serial_print("\e[0E");

          //Set cursor back to originial position
          int j = 0;
          for (j = 0; j < location; j++) {
            serial_print("\e[1C");
          }

          //Decrement length
          length--;

          return location;

        }
      }

      //Enter
      if (letter == (0x0A)) {
        return -1;
      }

      //Enter
      if (letter == (0x0D)) {
        return -1;
      }

      //Space
      if (letter == 0x20) {

        //Place letter into location
        buffer[location] = letter;

        //echo letter
        serial_print(&buffer[location]);

        //Increase location
        ++location;
        ++length;

        return location;

      }
      //Lowercase a-z
      if (letter >= (0x61) && letter <= (0x7A)){

        //Place letter into location
        buffer[location] = letter;

        //echo letter
        serial_print(&buffer[location]);

        //Increase location
        ++location;
        ++length;

        return location;

      }
      //Lowercase A-Z
      if (letter >= (0x41) && letter <= (0x5A)){
        //Place letter into location
        buffer[location] = letter;

        //echo letter
        serial_print(&buffer[location]);

        //Increase location
        ++location;

        return location;

      }
      //Number
      if (letter >= (0x30) && letter <=(0x39)){
        //Place letter into location
        buffer[location] = letter;

        //Echo number
        serial_print(&buffer[location]);

        //Increase location
        ++location;

        return location;
      }
  }//End numbers and letters

  return 0;

}
