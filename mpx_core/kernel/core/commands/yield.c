#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/command_handler.h>
#include <core/serial.h>
#include <include/string.h>
#include <core/commands/yield.h>
#include <modules/mpx_supt.h>
#include <core/PCB.h>
#include <core/struct.h>


int run_yield() {

  asm volatile ("int $60");
  return 0;

}
