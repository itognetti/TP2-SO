#ifndef __IDT_LOADER_H__
#define __IDT_LOADER_H__

#include <stdint.h>
#include <defs.h>
#include <interrupts.h>

static void setup_IDT_entry (int index, uint64_t offset);
void load_idt();

#endif