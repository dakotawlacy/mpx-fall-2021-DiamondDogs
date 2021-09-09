#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H

/*
  Procedure..: run_ch
  Description..: Main command handler loop that checks for what command has been
  entered
*/
int run_ch();

/*
  Procedure..: get_command
  Description..: Parses the command buffer and determines what command was
  typed in
  Params..: char* commandBuff, int bufferSize
*/
int get_command(char * commandBuff, int bufferSize);

#endif
