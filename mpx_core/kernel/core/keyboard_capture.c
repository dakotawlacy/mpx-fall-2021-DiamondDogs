#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/keyboard_capture.h>
#include <core/command_handler.h>
#include <core/serial.h>

int keyCap(char* buffer, int location, int length) {

    char letter = inb(COM1);
    length = strlen(buffer);
    //Check to see if ESC is pressed first
    if (letter == 0x1B) {
      //Get next char
      letter = inb(COM1);
      //Check to see if [ is pressed
      if (letter == 0x5B) {
        //Get next char
        letter = inb(COM1);
        //Check to see what arrow key is pressed
        if (letter == 0x44){

           if (location == 0) {
              //Do nothing
               return location;
           } else {
             //Move Location left one in logic
             location = location - 1;
             //Move location on screen left
             serial_print("\e[1D");
             //Reset buffer
             letter = inb(COM1);

             return location;
           }
        }
        else if (letter == 0x43){

          if (location == strlen(buffer)) {
            //Do nothing
            return location;
          }
          else {

            location = location + 1;

            serial_print("\e[1C");

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
    else  {

      //Backspace
      if (letter == (0x08)) {

        //Set back location
        --location;
        --length;

        return location;

      }

      //Backspace - NEED TO CHECK IF AT END, BEGINNING OR MIDDLE
      if (letter == (0x7F)) {

        //Check if at beginning
        if (location == 0) {
          //do nothing
          return location;
        }
        //Check if end
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

        }
        //In the middle
        else {

          //Move left in location
          --location;


          //Visually move left
          serial_print("\e[1D");
          //Counters
          int i = 0;
          int j = 0;
          int y = 0;
          //Index from current position to end
          while (i < (length - location)) {
            //Make temp char
            char temp = buffer[location + i + 1];
            //Place temp into location
            buffer[location + i] = temp;
            //increment counters
            i++;
            j++;
            //Echo char
            serial_print(&temp);
          }

          //Delete last character
          serial_print(" ");

          //Make end a null pointer
          buffer[length - 1] = '\0';

          //Put cursor back to original spot
          while (y < j) {
            serial_print("\e[1D");
            y++;
          }

          //Return locaiton
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
        serial_print(&letter);

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
        //Echo number
        serial_println("Number");

        //Place letter into location
        buffer[location] = letter;

        //Increase location
        ++location;

      }
  }//End numbers and letters

  return 0;

}
