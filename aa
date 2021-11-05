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

list heapList;

void initHeap() {

  //Inintialize free and alloc list
  initLists();

  heapAddress = kmalloc(1000 + sizeof(struct MCB));

  struct MCB* heap;
  heap = (MCB*)heapAddress;
  heap->size = 1000;
  heap->type = 0;//free
  heap->address = heapAddress;//Starting address
  strcpy(heap->pcb_name,"initial");

  if (heapList.head == NULL) {

    heapList.head = heap;
    heap->next = NULL;
    heap->prev = NULL;

  }

  allocateMem(100);
  allocateMem(200);
  printNodes();

}

void initLists() {

  heapList.head = NULL;
  heapList.count = 0;

}

//Allocate memory
void allocateMem(int size) {

  //Find MCB that has the space
  MCB* freeMCB = findSpace(size);
  //Temp size
  int temp = freeMCB->size;

  serial_println(freeMCB->pcb_name);

  //Set address of new MCB to start of free MCB
  struct MCB* newMCB;
  newMCB = (MCB*)freeMCB->address;

  //Change free MCB to new address
  freeMCB = (MCB*)(heapAddress + size + sizeof(struct MCB));
  //Set address
  freeMCB->address = newMCB->address + size + sizeof(struct MCB);

  // char* wow = "";
  // serial_println(itoa(freeMCB->address,wow));
  // wow = "";
  // serial_println(itoa(newMCB->address,wow));

  newMCB->size = size;//Set size to allocation size
  newMCB->type = 1;//Set to allocated
  newMCB->address = freeMCB->address;

  freeMCB->address = freeMCB->address + size + sizeof(struct MCB);
  freeMCB->size = temp - size - sizeof(struct MCB);

  newMCB->next = freeMCB;
  newMCB->prev = freeMCB->prev;
  freeMCB->prev = newMCB;

  //Check to see if new head is null


 }
struct MCB* findSpace(int size) {

  MCB* curr = heapList.head;

  while (curr != NULL) {
    //Found Space in a free MCB
    if (size < curr->size && curr->type == 0) {
      serial_println("Found Space");
      return curr;
    } else {
      curr = curr->next;
    }
  }

  return NULL;


}

void printNodes() {

  MCB* curr = heapList.head;
  char* swag = "";

  while (curr != NULL) {
    serial_println(itoa(curr->size,swag));
    curr = curr->next;
  }
}
