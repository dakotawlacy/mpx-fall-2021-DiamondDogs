#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/PCB.h>
#include <core/struct.h>

queue readyQueue;
queue blockedQueue;
queue suspendedReady;
queue suspendedBlock;

struct PCB* allocatePCB() {

  struct PCB* temp = sys_alloc_mem(sizeof(PCB));

  return temp;
}

 int freePCB(struct PCB* pcb){
   sys_free_mem(pcb);
   return 1;
 }

void get_pcb_data(char* commandBuff) {

 char name[15];
 memset(name,'\0',14);
 int i = 10;
 for (i = 10; i <= 24; i++) {
   if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
     char temp = commandBuff[i];
     name[i - 10] = temp;
   }
   else {
     break;
   }
 }

 name[14] = '\0';

 int classI;
 int pri;

 char class[2];
 memset(class, '\0', 1);
 class[0] = commandBuff[i+1];
 class[1] = '\0';
 classI = atoi(&class[0]);

 char priority[2];
 memset(priority, '\0', 1);
 priority[0] = commandBuff[i+3];
 priority[1] = '\0';
 pri = atoi(&priority[0]);

 setupPCB(name, classI, pri);

}

struct PCB* setupPCB(char * name, int class, int priority){

    //Create empty PCB
    struct PCB *newPCB;

    //Allocate PCB
    newPCB = allocatePCB();

    //Place data into PCB
    strcpy(newPCB->process_name, name);
    newPCB->process_class = class;
    newPCB->priority = priority;
    newPCB->state = 1;
    newPCB->susState = 0;

    //Place PCB into correct queue
    insertPCB(newPCB);

    return newPCB;
}

struct PCB* insertPCB(struct PCB* pcb){


  //MAKE SURE TO MAKE THE TWO QUEUES DIFFERENT PRIORITY AND FIFO!!!!!!!!!!!!!!!!!
    struct PCB *curr;
    struct queue *currQ;

    if (pcb->state == 1) {//Ready State
      if (pcb->susState == 0) {//Not suspended
        curr = readyQueue.head;
        currQ = &readyQueue;
      } else {//suspended
        curr = suspendedReady.head;
        currQ = &suspendedReady;
      }
    } else if (pcb->state == 2){//Blocked state
      if (pcb->susState == 0) {//Not suspended
        curr = blockedQueue.head;
        currQ = &blockedQueue;
      } else {//suspended
        curr = suspendedBlock.head;
        currQ = &suspendedBlock;
      }
    }

    struct PCB *temp;
    //Ready Or Blocked
    //Check if name is already in use

    if (findPCB(pcb->process_name) != NULL) {

      return NULL;//Already node with same name
    };

    //check if queue is empty
    if (readyQueue.head == NULL) {
      currQ->head = pcb;
      pcb->next = NULL;
      currQ->tail = pcb;
      serial_println("added at head");
      return pcb;
    }

    //Check if one value is in queue
    if (curr->next == NULL) {
      //If prio is same or greater
      if (curr->priority >= pcb->priority) {

        curr->next = pcb;
        pcb->next = NULL;
        pcb->previous = curr;
        currQ->tail = pcb;
        serial_println("Added at tail");
        return pcb;


      } else {
        serial_println("Added at head");
        pcb->next = curr;
        currQ->head = pcb;
        return pcb;

      }
    }

    //Traversal
    while (curr != NULL) {

        //Traverse
        if (curr->priority >= pcb->priority) {
          temp = curr;
          curr = curr->next;
          curr->previous = temp;

        } else {//
            if (curr->previous == NULL) {//Adding at head
              currQ->head = pcb;
              pcb->next = curr;
              curr->previous = pcb;
              serial_println("added at head");
              return pcb;
            }

          pcb->next = curr;
          curr->previous->next = pcb;
          pcb->previous = curr->previous;
          curr->previous = pcb;
          serial_println("added between ");
          serial_print(pcb->previous->process_name);
          serial_print(" and ");
          serial_print(pcb->next->process_name);
          return pcb;
        }

    }

    //If at end of queue
    temp = curr->previous;
    curr->previous->next = pcb;
    pcb->next = NULL;
    pcb->previous = temp;
    currQ->tail = pcb;
    serial_println("Added at tail");


    return pcb;

}

//Print Ready Queue
void printReady() {
  struct PCB *q = readyQueue.head;

  while (q != NULL) {
    serial_println(q->process_name);
    q = q->next;
  }
}

struct PCB* findPCB(char* name) {

   PCB* current;
   current = readyQueue.head;

   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       serial_println("Inside");
       return current;
     } else {
       current = current->next;

     }
   }

   current = suspendedReady.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       serial_println("Inside");
       return current;
     } else {
       current = current->next;

     }
   }

   current = blockedQueue.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       serial_println("Inside");
       return current;
     } else {
       current = current->next;

     }
   }

   current = suspendedBlock.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       serial_println("Inside");
       return current;
     } else {
       current = current->next;

     }
   }



   return NULL;

 }

void deletePCB(char* commandBuff) {

  //getName
  char name[15];
  memset(name,'\0',14);
  int i = 10;
  for (i = 10; i <= 24; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 10] = temp;
    }
    else {
      break;
    }
  }

  name[14] = '\0';


  struct PCB* pcb = findPCB(name);

  if (pcb != NULL) {
    serial_println("removing");
    removePCB(pcb);
  } else {
    serial_println("nothing to remove");
  }

}


int removePCB(struct PCB* pcb){

  struct PCB *curr;
  struct queue *currQ;

  if (pcb->state == 1) {//Ready State
    if (pcb->susState == 0) {//Not suspended
      curr = readyQueue.head;
      currQ = &readyQueue;
    } else {//suspended
      curr = suspendedReady.head;
      currQ = &suspendedReady;
    }
  } else if (pcb->state == 2){//Blocked state
    if (pcb->susState == 0) {//Not suspended
      curr = blockedQueue.head;
      currQ = &blockedQueue;
    } else {//suspended
      curr = suspendedBlock.head;
      currQ = &suspendedBlock;
    }
  }

  struct PCB *temp;

  //If node at beginning
  if (curr == pcb) {
    temp = currQ->head;
    currQ->head = curr->next;
    freePCB(temp);
    serial_println("removed at head");
    return 1;
  }

  while (curr != NULL) {
    temp = curr;
    curr = curr->next;
    curr->previous = temp;

    if (curr == pcb) {
      temp = curr;
      curr->previous->next = curr->next;
      serial_println("removing from middle");
      freePCB(temp);
      return 1;

    }

  }


  return 0;
}

void showPCB(char* commandBuff) {

  //getName
  char name[15];
  memset(name,'\0',14);
  int i = 8;
  for (i = 8; i <= 23; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 8] = temp;
    }
    else {
      break;
    }
  }

  name[14] = '\0';
  serial_println(name);
  struct PCB* pcb;
  pcb = findPCB(name);

  serial_println(pcb->process_name);

  char* temp = "";
  itoa(pcb->process_class,temp);

  serial_println(temp);

  itoa(pcb->state,temp);
  serial_println(temp);


  itoa(pcb->susState,temp);
  serial_println(temp);

  itoa(pcb->priority,temp);
  serial_println(temp);

}

 void initQueues() {
   readyQueue.head = NULL;
   readyQueue.tail = NULL;
   readyQueue.count = 0;

 }
