#include <ti/devices/msp/msp.h>
#include "FIFO2.h"


// Two-index implementation of the receive FIFO
// can hold 0 to RXFIFOSIZE-1 elements
uint32_t volatile PutI; // where to put next
uint32_t volatile GetI; // where to get next
#define ASCIIFIFOSIZE 64
char static ASCII_Fifo[ASCIIFIFOSIZE];


void ASCII_Fifo_Init(void){
  PutI = GetI = 0;  // empty
}

void ASCII_Fifo_Put(char data){
uint32_t newPutI = (PutI+1)&(ASCIIFIFOSIZE-1);
  if(newPutI == GetI){} ; // fail if full
  ASCII_Fifo[PutI] = data;          // save in Fifo
  PutI = newPutI;// next place to put
}
char ASCII_Fifo_Get(void){char data;
  if(GetI == PutI) return 0;      // fail if empty
  data = ASCII_Fifo[GetI];              // retrieve data
  GetI = (GetI+1)&(ASCIIFIFOSIZE-1); // next place to get
  return data;
}


int ASCII_Fifo_Size(void){
  return (PutI-GetI)&(ASCIIFIFOSIZE-1);
}

