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

list heapList;

void initHeap() {

  //Inintialize free and alloc list
  initLists();

  heapAddress = kmalloc(5000 + sizeof(MCB));

  struct MCB* heap;
  heap = (MCB*)heapAddress;
  heap->size = 5000;
  heap->type = 0;//free
  heap->address = heapAddress + sizeof(MCB);//Starting address
  strcpy(heap->pcb_name,"initial");

  if (heapList.head == NULL) {

    heapList.head = heap;
    heap->next = NULL;
    heap->prev = NULL;

  }

   //PCB* wow = (PCB*)allocateMem(100);

   //char* frigg = "";

  // PCB* now = (PCB*)allocateMem(4000);
  // PCB* what = (PCB*)allocateMem(3000);
  // PCB* mom = (PCB*)allocateMem(7000);
  // freeMem(wow);
  // freeMem(now);
  // //freeMem(what);
  // //freeMem(mom);
  //
  // if (wow) {
  //
  // }
  //
   //printNodes();

  return;

}

void initLists() {

  heapList.head = NULL;
  heapList.count = 0;

}

//Allocate memory
u32int allocateMem(u32int size) {

  //Find MCB that has enough space in it
  MCB* oldMCB = findSpace((int) size);

  //Temp Variables
  int tempSize = oldMCB->size;
  u32int oldNext = (u32int) oldMCB->next;
  u32int oldPrev = (u32int) oldMCB->prev;
  u32int tempAdd = oldMCB->address;

  char* oldName = "";
  strcpy(oldName,oldMCB->pcb_name);

  //Create new MCB block to create
  //Set new MCB equal to the location of the free block
  MCB* newMCB = oldMCB;

  //Change name
  strcpy(newMCB->pcb_name,"process");

  //Change type
  newMCB->type = 1;

  //Change size
  newMCB->size = size;

  newMCB->prev = (MCB*) oldPrev;

  //serial_println("fuck");
  //char* frigg = "";

  //serial_println(itoa((u32int)newMCB,frigg));
  //serial_println(itoa((u32int)tempAdd + size,frigg));



  //Edit Old block
  oldMCB = (MCB*)(tempAdd + size);

  //serial_println(itoa((u32int)oldMCB,frigg));
  /////////////////
  oldMCB->address = tempAdd + size + sizeof(MCB);

  //serial_println(itoa((u32int)oldMCB->address,frigg));
  /////////////////

  oldMCB->size = tempSize - size - sizeof(MCB);
  strcpy(oldMCB->pcb_name,oldName);

   newMCB->next = oldMCB;
   oldMCB->prev = newMCB;
   oldMCB->next = (MCB*) oldNext;

  //Add to heapList
  //Check for beginning
  if (newMCB == heapList.head) {
    heapList.head = newMCB;
  }

  return (u32int)newMCB->address;

 }

int freeMem(void* location) {

  //char* swag = "";

  //serial_println(itoa((u32int)location - sizeof(MCB),swag));
  //serial_println(itoa(heapAddress,swag));

  MCB* mcb = findMCB((u32int) location);
  mcb->type = 0;

  //serial_println(itoa((u32int)mcb,swag));
  //serial_println(mcb->next->pcb_name);

  //strcpy(mcb->pcb_name,"free");

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

  MCB* curr = heapList.head;

  while (curr != NULL) {

    if (curr->type == 0 && curr->next->type == 0) {

      curr->size = curr->size + curr->next->size + (int)sizeof(MCB);

      MCB* temp;

      temp = curr->next;

      curr->next = curr->next->next;

      temp = NULL;
      temp->next = NULL;
      temp->prev = NULL;

    }

    curr = curr->next;
  }

  return;
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
