#include <stdint.h>
#include <string.h>
#include <modules/mpx_supt.h>
#include <core/io.h>
#include <core/serial.h>
#include <core/PCB.h>
#include <core/mcb.h>
#include <core/struct.h>
#include <mem/heap.h>


u32int heapAddress = 0;
int allocNum = -1;
int tempSize;

list heapList;

void initHeap() {

  //Inintialize free and alloc list
  initLists();

  heapAddress = kmalloc(50000 + sizeof(struct MCB));

  struct MCB* heap;
  heap = (MCB*)heapAddress;
  heap->size = 50000;
  heap->type = 0;//free
  heap->address = heapAddress;//Starting address
  strcpy(heap->pcb_name,"initial");

  if (heapList.head == NULL) {

    heapList.head = heap;
    heap->next = NULL;
    heap->prev = NULL;

  }


  //Testing

  MCB* temp = allocateMem(100);
  //allocateMem(400,"proc2");
  MCB* temp2 = allocateMem(300);
  freeMem(temp->address);
  printNodes();
  // allocateMem(100,"proc4");
  // allocateMem(200,"proc5");
  //freeMem(temp2->address);
  if (temp2) {

  }
  // freeMem("proc4");
  // //freeMem("proc5");
  printNodes();
  // showAlloc();


}

void initLists() {

  heapList.head = NULL;
  heapList.count = 0;

}

//Allocate memory
MCB* allocateMem(int size) {

  //Find MCB that has the space
  MCB* freeMCB = findSpace(size);

  if (freeMCB == NULL) {
    return NULL;
  }

  //Temp values for free MCB
  int tempSize = freeMCB->size;
  u32int tempAddy = freeMCB->address;
  int tempType = freeMCB->type;
  char tempName[15];
  strcpy(tempName,freeMCB->pcb_name);

  if (tempAddy || tempSize) {

  }
  if (tempType) {

  }
  if (strcmp(tempName,"xxx")) {

  }

  //Set address of new MCB to start of free MCB
  struct MCB* newMCB;
  newMCB = (MCB*)freeMCB->address;

  newMCB->size = size;//Set size to allocation size
  newMCB->type = 1;//Set to allocated
  newMCB->address = freeMCB->address;
  strcpy(newMCB->pcb_name,cop->process_name);

  MCB* temp = freeMCB->next;

  int end = 0;
  if (temp != NULL) {

  } else {
    end = 1;
  }

  //Change free MCB to new address
  freeMCB = (MCB*)(newMCB->address + size + (int)sizeof(struct MCB));

  //Set address
  freeMCB->address = newMCB->address + size + (int)sizeof(struct MCB);



  strcpy(freeMCB->pcb_name,tempName);
  freeMCB->size = tempSize - size - (int)sizeof(struct MCB);
  freeMCB->address = tempAddy + size + sizeof(struct MCB);
  freeMCB->type = tempType;

  newMCB->next = freeMCB;
  newMCB->prev = freeMCB->prev;
  freeMCB->prev = newMCB;

  if (end) {
  } else {

    freeMCB->next = temp;
  }

  //Check to see if new head is null
  if (freeMCB == heapList.head) {
    //serial_println("swag");
    heapList.head = newMCB;
  }

  return newMCB;

 }

void freeMem(u32int location) {

  MCB* curr = (MCB*)location;
  curr->type = 0;
  strcpy(curr->pcb_name,"free");

  updateList();
  updateList();

}

void updateList() {

  MCB* curr = heapList.head;

  while (curr != NULL) {

    if (curr->type == 0 && curr->next->type == 0) {

      curr->size = curr->size + curr->next->size + (int)sizeof(MCB);

      MCB* temp;

      temp = curr->next;

      curr->next = curr->next->next;

      temp->next = NULL;
      temp->prev = NULL;

    }

    curr = curr->next;
  }


}

struct MCB* findSpace(int size) {

  MCB* curr = heapList.head;

  while (curr != NULL) {
    //Found Space in a free MCB
    if ((size + (int)sizeof(struct MCB)) < curr->size && curr->type == 0) {
      //serial_println("Found Space");
      return curr;
    } else {
      curr = curr->next;
    }
  }

  return NULL;


}

int isEmpty() {

  MCB* curr = heapList.head;


  while (curr != NULL) {

    if (curr->type == 1) {
      serial_println("Not empty");
      return 0;
    }

    curr = curr->next;
  }

  serial_println("empty");
  return 1;

}

void printNodes() {

  MCB* curr = heapList.head;
  char* swag = "";

  while (curr != NULL) {

    serial_println(curr->pcb_name);
    serial_println(itoa(curr->size,swag));
    swag = "";
    serial_println(itoa(curr->type,swag));
    serial_println(" ");
    curr = curr->next;
  }
}

void showFree() {

  MCB* curr = heapList.head;
  //char* swag = "";

  while (curr != NULL) {

    if (curr->type == 0) {
      serial_println(curr->pcb_name);
      //serial_println(itoa(curr->size,swag));
      //swag = "";
      //serial_println(itoa(curr->type,swag));
      //serial_println(" ");

    }
    curr = curr->next;
  }
}

void showAlloc() {

  MCB* curr = heapList.head;
  char* swag = "";

  while (curr != NULL) {

    if (curr->type == 1) {
      serial_println(curr->pcb_name);
      serial_println(itoa(curr->size,swag));
      swag = "";
      serial_println(itoa(curr->type,swag));
      serial_println(" ");

    }
    curr = curr->next;
  }
}
