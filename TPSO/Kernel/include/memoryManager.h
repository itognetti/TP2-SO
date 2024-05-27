#ifndef _MEM_MANAGER_H_
#define _MEM_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>
#include <mm_utils.h>

#define TRUE 1
#define FALSE 0


#define OFFSET_PTR(ptr, num) ((uint64_t) (ptr) + (num))
#define HP_ST ((void *) 0xA00000)  // 10 Mb
#define HP_SI 0x100000  // 1Mb   
#define HP_E ((void *) (OFFSET_PTR(HP_ST, HP_SI)))
 

void m_free(void * ptr);

memStatus * m_status();

void m_init();

void * m_malloc(uint64_t size);

#endif
