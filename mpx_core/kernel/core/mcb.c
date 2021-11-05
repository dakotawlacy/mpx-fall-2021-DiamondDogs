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

list freeList;
list allocList;

void initHeap() {

  //Inintialize free and alloc list
  initLists();

  heapAddress = kmalloc(1000 + sizeof(struct MCB));

  struct MCB* heap;
  heap = (MCB*)heapAddress;
  heap->size = 1000;
  heap->type = 0;//free
  heap->address = heapAddress;
  strcpy(heap->pcb_name,"initial");

  insertMCB(heap);

}

void initLists() {

  freeList.head = NULL;
  freeList.tail = NULL;
  freeList.count = 0;

  allocList.head = NULL;
  allocList.tail = NULL;
  allocList.count = 0;

}

void insertMCB(MCB* mcb) {

  if (mcb->type == 0) {
    insertFree(mcb);
  }
  else if (mcb->type == 1) {
    //insertAlloc();
  }


}

void insertFree(MCB* mcb) {

  //Insert at head
  if (freeList.head == NULL) {

    freeList.head = mcb;
    mcb->next = NULL;
    mcb->prev = NULL;

  }

  // //If one in freeList
  // if (freeList.head != NULL) {
  //
  //   //If head is less than
  //   if (freeList.head->address < mcb->address) {
  //     freeList.head->next = mcb;
  //     mcb->prev = freeList.head;
  //   } else {
  //
  //     mcb->next = freeList.head;
  //     freeList.head->prev = mcb;
  //     freeList.head = mcb;
  //
  //   }
  // }



}
//
// // void insertAlloc(MCB* mcb) {
// //
// // }
