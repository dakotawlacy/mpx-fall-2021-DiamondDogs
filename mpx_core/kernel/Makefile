#
# Makefile for MPX/Linux

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS =
ASFLAGS = -f elf -g

OBJFILES =\
core/gdt.o\
core/idt.o\
core/interrupts.o\
core/io.o\
core/irq.o\
core/kmain.o\
core/serial.o\
core/system.o\
core/tables.o\
mem/paging.o\
mem/heap.o\
core/command_handler.o\
core/keyboard_capture.o\
core/startup.o\
core/commands/help.o\
core/commands/shutdown.o\
core/commands/version.o\
core/commands/getdate.o\
core/commands/setdate.o\
core/commands/gettime.o\
core/commands/settime.o\
core/commands/yield.o\
core/loadr3.o\
core/testProcesses.o\
core/commands/alarm.o\
core/mcb.o\
core/PCB.o\
core/newserial.o\
core/dcb.o\
core/iocb.o\
core/newprocesses.o

.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -I../ -o $@ $<

all: kernel.o

kernel.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r -o kernel.o $(OBJFILES)

clean:
	rm $(OBJFILES)
