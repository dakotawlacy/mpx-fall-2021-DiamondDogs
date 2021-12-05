#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/PCB.h>
#include <core/mcb.h>
#include <core/struct.h>
#include <mem/heap.h>

PCB* cop;

u32int heapAddress = 0;
int allocNum = -1;
int tempSize;
char printx[32];
char * nl = "\n";
int nllen = 2;

list heapList;

void initHeap() {

  //Inintialize free and alloc list
  initLists();

  //Set starting heap address
  heapAddress = kmalloc(50000 + sizeof(MCB));

  //Create initial MCB
  struct MCB* heap;
  heap = (MCB*)heapAddress;
  heap->size = 50000;
  heap->type = 0;//free
  heap->address = heapAddress + sizeof(MCB);//Starting address

  //Place at head
  if (heapList.head == NULL) {

    heapList.head = heap;
    heap->next = NULL;
    heap->prev = NULL;

  }

  return;

}

void initLists() {

  heapList.head = NULL;
  heapList.count = 0;

}

u32int allocateMem(u32int size) {

  //Find MCB that has enough space in it
  MCB* freeMCB = findSpace((int) size);

  if (!freeMCB) {
    return NULL;
  }

  //Temp Variables
  int tempSize = freeMCB->size;
  u32int oldNext = (u32int) freeMCB->next;
  u32int tempAdd = freeMCB->address;

  //Create new MCB block to create
  //Set new MCB equal to the location of the free block
  MCB* allocatedMCB = freeMCB;

  //Change type
  allocatedMCB->type = 1;

  //Change size
  allocatedMCB->size = size;

  //Edit Old block
  freeMCB = (MCB*)(tempAdd + size);

  //Set new address of the freeMCB
  freeMCB->address = tempAdd + size + sizeof(MCB);

  //Change size of free block
  freeMCB->size = tempSize - size - sizeof(MCB);

  //Set up pointers
  allocatedMCB->next = freeMCB;
  freeMCB->prev = allocatedMCB;
  freeMCB->next = (MCB*) oldNext;

  //return address
  return (u32int)allocatedMCB->address;

 }

int freeMem(void* location) {

  //Find MCB to be freed
  MCB* mcb = findMCB((u32int) location);

  //Change type to free
  mcb->type = 0;

  //Update the list
  updateList();
  updateList();

  return 0;

}

MCB* findMCB(u32int address) {



  MCB* curr = heapList.head;

  while (curr != NULL) {

    if (curr->address == address) {

      return curr;
    }

    curr = curr->next;
  }
  return NULL;
}

void updateList() {

  //Set MCB* for traversal
  MCB* curr = heapList.head;

  //Traverse list
  while (curr != NULL) {

    //Check for two nodes next to each other
    if (curr->type == 0 && curr->next->type == 0) {

      //Combine sizes
      curr->size = curr->size + curr->next->size + (int)sizeof(MCB);

      //Remove node
      MCB* temp;
      temp = curr->next;
      curr->next = curr->next->next;

      //Free node pointers
      temp = NULL;
      temp->next = NULL;
      temp->prev = NULL;

    }

    //Traverse
    curr = curr->next;
  }

  return;
}

struct MCB* findSpace(int size) {

  //Set node for traversal
  MCB* curr = heapList.head;

  //Traverse
  while (curr != NULL) {
    //Found Space in a free MCB
    if ((size + (int)sizeof(struct MCB)) < curr->size && curr->type == 0) {
      //serial_println("Found Space");
      return curr;
    } else {
      curr = curr->next;
    }
  }

  //If none is found
  return NULL;

}

int isEmpty() {

  MCB* curr = heapList.head;

  while (curr != NULL) {

    if (curr->type == 1) {
      return 0;
    }

    curr = curr->next;
  }

  return 1;

}

void printNodes() {

  MCB* curr = heapList.head;
  char* swag = "";

  while (curr != NULL) {

    serial_println(itoa(curr->size,swag));
    swag = "";
    serial_println(itoa(curr->type,swag));
    serial_println(itoa(curr->address,swag));
    serial_println(" ");
    curr = curr->next;
  }
}

void showFree() {

  MCB* curr = heapList.head;
  memset(printx,'\0',32);
  char * printptr = printx;
  int println = 0;

  while (curr != NULL) {

    if (curr->type == 0) {

      strcpy(printx,"Address: ");
      println = 9;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      itoa(curr->address,printptr);
      println = strlen(printptr);
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);
      memset(printx,'\0',32);

      strcpy(printx,"Type: ");
      println = 6;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      strcpy(printx,"Free");
      println = 4;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);
      memset(printx,'\0',32);

      strcpy(printx,"Size: ");
      println = 6;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      println = 0;

      itoa(curr->size,printptr);
      println = strlen(printptr);
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);

    }
    curr = curr->next;
  }
}

void showAlloc() {


  MCB* curr = heapList.head;
  memset(printx,'\0',32);
  char * printptr = printx;
  int println = 0;

  while (curr != NULL) {

    if (curr->type == 1) {

      strcpy(printx,"Address: ");
      println = 9;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      itoa(curr->address,printptr);
      println = strlen(printptr);
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);
      memset(printx,'\0',32);

      strcpy(printx,"Type: ");
      println = 6;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      strcpy(printx,"Allocated");
      println = 9;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);
      memset(printx,'\0',32);

      strcpy(printx,"Size: ");
      println = 6;
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);
      memset(printx,'\0',32);

      println = 0;

      itoa(curr->size,printptr);
      println = strlen(printptr);
      sys_req(WRITE,DEFAULT_DEVICE,printptr,&println);

      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);
      sys_req(WRITE,DEFAULT_DEVICE,nl,&nllen);


    }
    curr = curr->next;
  }

}
