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

//Allocate PCB Memory
struct PCB* allocatePCB() {

  //Allocate size
  struct PCB* temp = sys_alloc_mem(sizeof(PCB));

  //Return pcb
  return temp;
}

//Free PCB Memory
void freePCB(struct PCB* pcb){

  //Free memory
  sys_free_mem(pcb);

}

//Get PCB data
int get_pcb_data(char* commandBuff) {

  //Check name length
  if (strlen(commandBuff) > 29) {
    return 4000;
    serial_println("name too long");
  }

  //Get Name
  char name[16];
  memset(name,'\0',14);
  int i = 10;
  int j = i;
  for (i = 10; i <= j + 15; i++) {

    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      if (commandBuff[i] >= (0x30) && commandBuff[i] <= (0x39)) {
        serial_println("number in name");
        return 5000;
      }

      char temp = commandBuff[i];
      name[i - 10] = temp;
    }
    else {
      break;
    }
  }

  //Set NULL Point
  name[15] = '\0';

  //Error Checking
  //Two digit number
  if (commandBuff[i+2] != ' ' || commandBuff[i+4] != '\0') {
    serial_println("Two digit number");
    return 1000;//Invalid Number
  }

  //Invalid Class
  if (commandBuff[i + 1] < (0x31) || commandBuff[i + 1] > (0x32)) {
    serial_println("Invalid class");
    return 2000;//Not a number
  }

  //Invalid Priority
  if (commandBuff[i + 3] < (0x30) || commandBuff[i + 3] > (0x39)) {
    serial_println("Invalid priority");
    return 2000;//Not a number
  }

  //Initialize class and priority
  int classI;
  int pri;

  //Get class
  char class[2];
  memset(class, '\0', 1);
  class[0] = commandBuff[i+1];
  class[1] = '\0';
  classI = atoi(&class[0]);

  //Get priority
  char priority[2];
  memset(priority, '\0', 1);
  priority[0] = commandBuff[i+3];
  priority[1] = '\0';
  pri = atoi(&priority[0]);

  //Call setup
  if (setupPCB(name, classI, pri) == NULL) {
    return 3000;//Already Inside
  };

  return 1;

}

//Setup new pcb
struct PCB* setupPCB(char * name, int class, int priority){

    //Create empty PCB
    struct PCB *newPCB;

    //Check for duplicate name
    if (findPCB(name) != NULL) { return NULL; }

    //Allocate PCB
    newPCB = allocatePCB();

    //Place data into PCB
    strcpy(newPCB->process_name, name);
    newPCB->process_class = class;
    newPCB->priority = priority;
    newPCB->state = 1;//0 running, 1 ready, 2 blocked
    newPCB->susState = 0;
    memset(&newPCB->process_stack,'\0',1024);
    newPCB->stackBase = &newPCB->process_stack[0];
    newPCB->stackTop = newPCB->stackBase + 1024;

    //Place PCB into correct queue
    insertPCB(newPCB);

    return newPCB;
}

//Insert pcb into correct queue
struct PCB* insertPCB(struct PCB* pcb){

  //Initialize pcb's
  struct PCB *curr;
  struct queue *currQ;
  struct PCB *temp;

  //Check state and suspended state status
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

  //Check if name is already in use
  if (findPCB(pcb->process_name) != NULL) {
      return NULL;//Already node with same name
  };

  //If ready or suspendedReady
  if (pcb->state == 1) {

    //check if queue is empty
    if (currQ->head == NULL) {
      currQ->head = pcb;
      pcb->next = NULL;
      currQ->tail = pcb;
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
        //serial_println("Added at tail");
        return pcb;

      } else {
        //serial_println("Added at head");
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

      }
      else {//

        if (curr->previous == NULL) {//Adding at head
          currQ->head = pcb;
          pcb->next = curr;
          curr->previous = pcb;
          //serial_println("added at head");
          return pcb;
        }

        pcb->next = curr;
        curr->previous->next = pcb;
        pcb->previous = curr->previous;
        curr->previous = pcb;
        serial_print(pcb->previous->process_name);
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

    return pcb;
  }
  else {//If blocked or suspendedBlock

    //If queue is empty
    if (currQ->head == NULL) {
      currQ->head = pcb;
      pcb->next = NULL;
      currQ->tail = pcb;
      return pcb;
    } else {
      currQ->tail->next = pcb;
      currQ->tail = pcb;
      pcb->next = NULL;
      return pcb;
    }
  }
}

//Set new priority of pcb
void setPriority(char* commandBuff) {

  //getName
  char name[16];
  memset(name,'\0',15);
  int i = 12;
  int j = i;
  for (i = 12; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 12] = temp;
    }
    else {
      break;
    }
  }

  name[15] = '\0';

  //error checking
  if (commandBuff[i + 2] != '\0') {
    return;
    serial_println("invalid priority");
  }

  //initalize priority value
  int pri;

  //Get priority
  char priority[2];
  memset(priority, '\0', 1);
  priority[0] = commandBuff[i+1];
  priority[1] = '\0';
  pri = atoi(&priority[0]);

  //find pcb and save data
  struct PCB* pcb;
  pcb = findPCB(name);

  //remove pcb from queue
  removePCB(pcb);

  //set new priority
  pcb->priority = pri;

  //place pcb back into correct queue
  insertPCB(pcb);

}

//Print Ready Queue
void printReady() {

  struct PCB *q = readyQueue.head;

  serial_println("READY:");

  while (q != NULL) {
    serial_println(q->process_name);
    q = q->next;
  }

  serial_println("SUSPENDED READY;");

  q = suspendedReady.head;

  while (q != NULL) {
    serial_println(q->process_name);
    q = q->next;
  }
}

//Print Block Queue
void printBlock() {

  struct PCB *q = blockedQueue.head;

  serial_println("BLOCKED:");

  while (q != NULL) {
    serial_println(q->process_name);
    q = q->next;
  }

  serial_println("SUSPENDED BLOCK:");

  q = suspendedBlock.head;

  while (q != NULL) {
    serial_println(q->process_name);
    q = q->next;
  }

}

void printAll() {
  printReady();
  printBlock();
}

struct PCB* findPCB(char* name) {

   PCB* current;
   current = readyQueue.head;

   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }

   current = suspendedReady.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }

   current = blockedQueue.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {

       return current;
     } else {
       current = current->next;

     }
   }

   current = suspendedBlock.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }



   return NULL;

 }

void deletePCB(char* commandBuff) {

  //getName
  char name[16];
  memset(name,'\0',15);
  int i = 10;
  int j = i;
  for (i = 10; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 10] = temp;
    }
    else {
      break;
    }
  }

  name[15] = '\0';


  struct PCB* pcb = findPCB(name);

  if (pcb != NULL) {
    serial_println("removed + name");
    removePCB(pcb);
  } else {
    serial_println("pcb does not exist");
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

    return 1;
  }

  while (curr != NULL) {
    temp = curr;
    curr = curr->next;
    curr->previous = temp;

    if (curr == pcb) {
      temp = curr;
      curr->previous->next = curr->next;

      freePCB(temp);
      return 1;

    }

  }


  return 0;
}

void showPCB(char* commandBuff) {

  //getName
  char name[16];
  memset(name,'\0',15);
  int i = 8;
  int j = i;
  for (i = 8; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 8] = temp;
    }
    else {
      break;
    }
  }

  name[15] = '\0';
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

struct PCB* blockPCB(char* commandBuff) {

  //Get Name
  char name[16];
  memset(name,'\0',14);
  int i = 9;
  int j = i;
  for (i = 9; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 9] = temp;
    }
    else {
      break;
    }
  }
  //Set NULL Point
  name[15] = '\0';


  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->state = 2;

  insertPCB(pcb);

  return pcb;


}

struct PCB* unblockPCB(char* commandBuff) {

  //Get Name
  char name[16];
  memset(name,'\0',15);
  int i = 11;
  int j = i;
  for (i = 11; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 11] = temp;
    }
    else {
      break;
    }
  }
  //Set NULL Point
  name[15] = '\0';

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->state = 1;

  insertPCB(pcb);

  return pcb;

}

struct PCB* suspendPCB(char* commandBuff) {

  //Get Name
  char name[16];
  memset(name,'\0',15);
  int i = 11;
  int j = i;
  for (i = 11; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 11] = temp;
    }
    else {
      break;
    }
  }
  //Set NULL Point
  name[15] = '\0';

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->susState = 1;

  insertPCB(pcb);

  return pcb;

}

struct PCB* resumePCB(char* commandBuff) {

  //Get Name
  char name[16];
  memset(name,'\0',15);
  int i = 10;
  int j = i;
  for (i = 10; i <= j + 15; i++) {
    if (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
      char temp = commandBuff[i];
      name[i - 10] = temp;
    }
    else {
      break;
    }
  }
  //Set NULL Point
  name[15] = '\0';

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->susState = 0;

  insertPCB(pcb);

  return pcb;

}

void initQueues() {
   readyQueue.head = NULL;
   readyQueue.tail = NULL;
   readyQueue.count = 0;

 }
