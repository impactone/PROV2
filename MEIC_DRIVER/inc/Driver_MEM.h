#ifndef _Driver_MEM_H
#define _Driver_MEM_H
#include "stm32f4xx.h"
#include "stdio.h"
struct MEM_Control;
void* MEM_malloc(u16 msize);
u8   MEM_free(u8 *p);
u8  MEM_Used(void);
//void *mymalloc(u32 size);
//void myfree(void *ptr);
#endif