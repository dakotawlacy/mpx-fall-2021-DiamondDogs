#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/PCB.h>
#include <core/struct.h>

#define SUCCESS 1;
#define INVALIDNAME 1000;
#define INVALIDCLASS 2000;
#define ALREADYINSIDE 3000;
#define DOESNOTEXIST 4000;

queue readyQueue;
queue blockedQueue;
queue suspendedReady;
queue suspendedBlock;
int newLine = 2;



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
    char* invName = "Invalid Name\n";
    int invName_len = strlen(invName);
    sys_req(WRITE,DEFAULT_DEVICE,invName,&invName_len);
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
    char* invName = "Invalid Name\n";
    int invName_len = strlen(invName);
    sys_req(WRITE,DEFAULT_DEVICE,invName,&invName_len);
    return NULL;
  }

  i++;
  //Get to second space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    char* invClass = "Invalid Class\n";
    int invClass_len = strlen(invClass);
    sys_req(WRITE,DEFAULT_DEVICE,invClass,&invClass_len);
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
    char* invName = "Invalid Name\n";
    int invName_len = strlen(invName);
    sys_req(WRITE,DEFAULT_DEVICE,invName,&invName_len);
    return NULL;
  }

  i++;
  //Get to second space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    char* invClass = "Invalid Class\n";
    int invClass_len = strlen(invClass);
    sys_req(WRITE,DEFAULT_DEVICE,invClass,&invClass_len);
    return NULL;
  }

  i++;

  //Get to third space
  while (commandBuff[i] != ' ' && commandBuff[i] != '\0') {
    i++;
  }

  if (commandBuff[i + 1] == ' ' || commandBuff[i + 1] == '\0') {
    char* invPrio = "Invalid Priority\n";
    int invPrio_len = strlen(invPrio);
    sys_req(WRITE,DEFAULT_DEVICE,invPrio,&invPrio_len);
    return NULL;
  }

  i++;

  if (commandBuff[i + 1] != '\0') {
    char* invPrio = "Priority Too Big\n";
    int invPrio_len = strlen(invPrio);
    sys_req(WRITE,DEFAULT_DEVICE,invPrio,&invPrio_len);
    return NULL;
  }

  if (commandBuff[i] < (0x30) || commandBuff[i] > (0x39) ) {
    char* invPrio = "Priority is not a number.\n";
    int invPrio_len = strlen(invPrio);
    sys_req(WRITE,DEFAULT_DEVICE,invPrio,&invPrio_len);
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
  sys_free_mem(pcb->stackBase);

}

//Get PCB data
int get_pcb_data(char* commandBuff) {

  if (get_name(commandBuff) == NULL || get_class(commandBuff) == NULL || get_prio(commandBuff) == NULL) {
    return INVALIDNAME;//invalid name;
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

  if (classI != 1 && classI != 2) {
    return INVALIDCLASS;
  }

  //Call setup
  if (setupPCB(name, classI, pri) == NULL && readyQueue.head != NULL) {
    return ALREADYINSIDE;//Already Inside
  };

  return SUCCESS;

}

//Setup new pcb
struct PCB* setupPCB(char * name, int class, int priority){

    //Create empty PCB
    struct PCB *newPCB;

    //Check for duplicate name
    if (findPCB(name) != NULL) {
      return NULL;
    }

    //Allocate PCB
    newPCB = allocatePCB();

    //Place data into PCB
    strcpy(newPCB->process_name, name);
    newPCB->process_class = class;
    newPCB->priority = priority;
    newPCB->state = 1;//0 running, 1 ready, 2 blocked
    newPCB->susState = 1;
    memset(&newPCB->process_stack,'\0',1024);
    newPCB->stackBase = &newPCB->process_stack[0];
    newPCB->stackTop = newPCB->stackBase + 1024 - sizeof(struct context);
    newPCB->cont = (context*)newPCB->stackTop;
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
        return pcb;

      } else {
        pcb->next = curr;
        currQ->head = pcb;
        return pcb;

      }
    }

    //Add at head
    if (curr->priority < pcb->priority) {
      pcb->next = curr;
      currQ->head = pcb;
      curr->previous = pcb;
      pcb->previous = NULL;
      return pcb;
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

        // if (curr->previous == NULL) {//Adding at head
        //   currQ->head = pcb;
        //   pcb->next = curr;
        //   curr->previous = pcb;
        //   pcb->previous = NULL;
        //   return pcb;
        // }

        pcb->next = curr;
        curr->previous->next = pcb;
        pcb->previous = curr->previous;
        curr->previous = pcb;
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

  return SUCCESS;

}

//Print individual pcb
void print(PCB* q) {

  while (q != NULL) {

    char* nl = "\n";
    int nl_len = strlen(nl);

    char* temp = "";
    int temp_len = 0;

    temp = "------------";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "";
    temp_len = 0;

    temp = "Process Name: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = "";
    temp_len = 0;

    temp = q->process_name;
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = "";
    temp_len = 0;

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);


    temp = "Process Class: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = "";
    temp_len = 0;

    if (q->process_class == 1) {
      temp = "System";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);


    } else {
      temp = "Application";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }

    temp = "";
    temp_len = 0;

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Process State: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = "";
    temp_len = 0;

    if (q->state == 1) {
      temp = "Ready";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    } else if (q->state == 2){
      temp = "Blocked";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }
    temp = "";
    temp_len = 0;

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Suspended State: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    temp = "";

    if (q->susState == 0) {

      temp = "Not Suspended";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    } else if (q->state == 1){
      temp = "Suspended";
      temp_len = strlen(temp);
      sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);
    }

    temp = "";
    temp_len = 0;

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    temp = "Priority: ";
    temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    temp = "";
    temp_len = 0;

    char* prio = "";
    itoa(q->priority,prio);
    temp_len = strlen(prio);
    sys_req(WRITE,DEFAULT_DEVICE,prio,&temp_len);

    temp = "";
    temp_len = 0;

    //New line
    sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

    q = q->next;
  }
}

//Print Ready Queue
void printReady() {

  struct PCB *q = readyQueue.head;

  char* message = "\x1b[1;30mReady Queue:\x1b[1;0m\n";
  int message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);

  print(q);

  char* temp = "------------\n";
  int temp_len = strlen(temp);
  sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

  q = suspendedReady.head;
  message = "\x1b[1;30mSuspended Ready Queue:\x1b[1;0m\n";
  message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);
  print(q);


}

//Print Block Queue
void printBlock() {

  struct PCB *q = blockedQueue.head;
  char* message = "\x1b[1;30mBlocked Queue:\x1b[1;0m\n";
  int message_length = strlen(message);
  sys_req(WRITE,DEFAULT_DEVICE,message,&message_length);

  print(q);

  q = suspendedBlock.head;
  message = "\x1b[1;30mSuspended Blocked Queue:\x1b[1;0m\n";
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
  //char name[16];
  //strcpy(name,get_name(commandBuff));

  struct PCB* pcb = findPCB(commandBuff);

  if (pcb != NULL) {
  //  char* remove = "Removed ";
  //  int remName_len = strlen(remove);
  //  sys_req(WRITE,DEFAULT_DEVICE,remove,&remName_len);
    //int pcbNameLen = strlen(pcb->process_name);
    //sys_req(WRITE, DEFAULT_DEVICE, pcb->process_name, &pcbNameLen);
    sys_req(WRITE, DEFAULT_DEVICE, "\n", &newLine);
    removePCB(pcb);
  } else {
    char* noExist = "PCB does not exist.\n";
    int noExist_len = strlen(noExist);
    sys_req(WRITE,DEFAULT_DEVICE,noExist,&noExist_len);
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
    return SUCCESS;
  }

  //Last one
  if (curr->next == NULL) {
    currQ->head = NULL;
    currQ->tail = NULL;
    freePCB(curr);
    return SUCCESS;
  }
  //Traverse
  while (curr != NULL) {
    temp = curr;
    curr = curr->next;
    curr->previous = temp;

    if (curr == pcb) {
      temp = curr;
      curr->previous->next = curr->next;

      freePCB(temp);
      return SUCCESS;

    }
  }

  return SUCCESS;
}

//Show contents of PCB
int showPCB(char* commandBuff) {

  char* nl = "\n";
  int nl_len = strlen(nl);

  //getName
  char name[16];
  strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(name);


  //If doesn't exist
  if (pcb == NULL) {

    char* temp = "Does not exist\n";
    int temp_len = strlen(temp);
    sys_req(WRITE,DEFAULT_DEVICE,temp,&temp_len);

    return DOESNOTEXIST;
  }

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
  sys_req(WRITE,DEFAULT_DEVICE,nl,&nl_len);

  return SUCCESS;

}

struct PCB* blockPCB(char* commandBuff) {

  //getName
  char name[16];
  strcpy(name,get_name(commandBuff));

  if(findPCB(name) == NULL){
    char* temp = "Invalid Name\n";
    int temp1 = strlen(temp);
    sys_req(WRITE, DEFAULT_DEVICE, temp, &temp1);
    return NULL;
  }

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

  if(findPCB(name) == NULL){
    char* temp = "Invalid Name\n";
    int temp1 = strlen(temp);
    sys_req(WRITE, DEFAULT_DEVICE, temp, &temp1);
    return NULL;
  }

  struct PCB* pcb;
  pcb = findPCB(name);

  removePCB(pcb);

  pcb->state = 1;

  insertPCB(pcb);

  return pcb;

}

struct PCB* suspendPCB(char* commandBuff) {

  //getName
  //char name[16];

  //strcpy(name,get_name(commandBuff));

  struct PCB* pcb;
  pcb = findPCB(commandBuff);

  removePCB(pcb);

  pcb->susState = 1;

  insertPCB(pcb);

  return pcb;

}

void resumeAll(){

  PCB* resume;
  resume = suspendedReady.head;


  while (resume != NULL) {

    removePCB(resume);

    resume->susState = 0;
    serial_println(resume->process_name);
    insertPCB(resume);

    resume = resume->next;
  }





  return;
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
