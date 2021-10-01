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

//Get Name
char* get_name(char* commandBuff) {

  //Get Name
  char name[16];
  memset(name,'\0',16);

  //Get to the first space;
  int i = 0;
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Name");
    return NULL;
  }

  //Set value to first letter
  i++;
  int j = 0;
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {

    if (j == 16) {
      break;
    }

    char temp = commandBuff[i];
    name[j] = temp;

    j++;
    i++;
  }

  //Set NULL Point
  name[15] = '\0';

  char* name_ptr = name;

  return name_ptr;

}

char* get_class(char* commandBuff) {

  //Get class
  char class[2];
  memset(class,'\0',2);

  //Get to the first space;
  int i = 0;
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Name");
    return NULL;
  }

  i++;
  //Get to second space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Cla444ss");
    return NULL;
  }


  i++;


  class[0] = commandBuff[i];

  char* class_ptr = class;

  return class_ptr;

}

char* get_prio(char* commandBuff) {

  char prio[2];
  memset(prio,'\0',2);

  int i = 0;
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Name");
    return NULL;
  }

  i++;
  //Get to second space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Cla222ss");
    return NULL;
  }

  i++;

  //Get to third space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    serial_println("Invalid Prio");
    return NULL;
  }

  i++;

  if (commandBuff[i + 1] != '\0') {
    serial_println("Prio too big");
    return NULL;
  }

  if (commandBuff[i] < (0x30) || commandBuff[i] > (0x39) ) {
    serial_println("prio not a number");
    return NULL;
  }

  prio[0] = commandBuff[i];

  char* prio_ptr = prio;

  return prio_ptr;

}

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

  if (get_name(commandBuff) == NULL || get_class(commandBuff) == NULL || get_prio(commandBuff) == NULL) {
    return 8000;//invalid name;
  }

  char name[16];
  char class[2];
  char prio[2];

  strcpy(name,get_name(commandBuff));
  strcpy(class,get_class(commandBuff));
  strcpy(prio,get_prio(commandBuff));


  //Initialize class and priority
  int classI;
  int pri;

  classI = atoi(class);
  pri = atoi(prio);

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
int setPriority(char* commandBuff) {

  //getName
  char name[16];
  char prio[2];

  strcpy(name,get_name(commandBuff));
  strcpy(prio,get_class(commandBuff));

  //initalize priority value
  int pri;
  pri = atoi(prio);

  //find pcb and save data
  struct PCB* pcb;
  pcb = findPCB(name);

  //remove pcb from queue
  removePCB(pcb);

  //set new priority
  pcb->priority = pri;

  //place pcb back into correct queue
  insertPCB(pcb);

  return 1;

}

void print(PCB* q) {

  while (q != NULL) {

    char* nl = "\n";
    int nl_len = strlen(nl);

    char* temp = "------------";
    int temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Process Name: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = q->process_name;
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);


    temp = "Process Class: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    if (q->process_class == 1) {
      temp = "System";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    } else {
      temp = "Application";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Process State: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);


    if (q->state == 1) {
      temp = "Ready";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    } else if (q->state == 2){
      temp = "Blocked";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Suspended State: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);


    if (q->susState == 0) {
      temp = "Not Suspended";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    } else if (q->state == 1){
      temp = "Suspended";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Priority: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    char* prio = "";
    itoa(q->priority,prio);
    temp_len = strlen(prio);
    sys_req(WRITE,DEFAULT_DEVICE,prio,&temp_len);

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);
    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    q = q->next;
  }
}


//Print Ready Queue
void printReady() {

  struct PCB *q = readyQueue.head;
  char* message = "Ready Queue:\n";
  int message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);

  print(q);

  q = suspendedReady.head;
  message = "Suspended Ready Queue:\n";
  message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);
  print(q);


}

//Print Block Queue
void printBlock() {

  struct PCB *q = blockedQueue.head;
  char* message = "Blocked Queue:\n";
  int message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);

  print(q);

  q = suspendedBlock.head;
  message = "Suspended Blocked Queue:\n";
  message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);
  print(q);

}

//Print Every Queue
void printAll() {
  printReady();
  printBlock();
}

//Find PCB in all queues
struct PCB* findPCB(char* name) {

   PCB* current;

   //Search ready
   current = readyQueue.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }

   //Search suspended Ready
   current = suspendedReady.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }

   //Search blocked
   current = blockedQueue.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {

       return current;
     } else {
       current = current->next;

     }
   }

   //Search Suspended Block
   current = suspendedBlock.head;
   while (current != NULL) {
     if (strcmp(current->process_name,name) == 0) {
       return current;
     } else {
       current = current->next;

     }
   }
   //Not inside
   return NULL;

 }

//Delete pcb from queue
void deletePCB(char* commandBuff) {

  //getName
  char name[16];
  strcpy(name,get_name(commandBuff));

  struct PCB* pcb = findPCB(name);

  if (pcb != NULL) {
    serial_println("removed + name");
    removePCB(pcb);
  } else {
    serial_println("pcb does not exist");
  }
}

//Remove PCB from correct queue
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

//Show contents of PCB
void showPCB(char* commandBuff) {

  char* nl = "\n";
  int nl_len = strlen(nl);

  //getName
  char name[16];
  strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(name);

  char* temp = "Process Name: ";
  int temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  temp = pcb->process_name;
  temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  //New line
  sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);


  temp = "Process Class: ";
  temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  if (pcb->process_class == 1) {
    temp = "System";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  } else {
    temp = "Application";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  }

  //New line
  sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

  temp = "Process State: ";
  temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);


  if (pcb->state == 1) {
    temp = "Ready";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  } else if (pcb->state == 2){
    temp = "Blocked";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  }

  //New line
  sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

  temp = "Suspended State: ";
  temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);


  if (pcb->susState == 0) {
    temp = "Not Suspended";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  } else if (pcb->state == 1){
    temp = "Suspended";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
  }

  //New line
  sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

  temp = "Priority: ";
  temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  char* prio = "";
  itoa(pcb->priority,prio);
  temp_len = strlen(prio);
  sys_req(WRITE,DEFAULT_DEVICE,prio,&temp_len);

}

struct PCB* blockPCB(char* commandBuff) {

  //getName
  char name[16];
  strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->state = 2;

  insertPCB(pcb);

  return pcb;
}

struct PCB* unblockPCB(char* commandBuff) {

  //getName
  char name[16];

  strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->state = 1;

  insertPCB(pcb);

  return pcb;

}

struct PCB* suspendPCB(char* commandBuff) {

  //getName
  char name[16];

  strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->susState = 1;

  insertPCB(pcb);

  return pcb;

}

struct PCB* resumePCB(char* commandBuff) {

  //getName
  char name[16];

  strcpy(name,get_name(commandBuff));

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

   suspendedReady.head = NULL;
   suspendedReady.tail = NULL;
   suspendedReady.count = 0;

   blockedQueue.head = NULL;
   blockedQueue.tail = NULL;
   blockedQueue.count = 0;

   suspendedBlock.head = NULL;
   suspendedBlock.tail = NULL;
   suspendedBlock.count = 0;


 }
