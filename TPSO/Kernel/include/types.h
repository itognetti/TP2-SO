#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>

typedef struct processInfo{
    char * name;
    unsigned int id;
    uint8_t state;
    uint8_t priority;
    uint64_t stack;
    uint64_t rsp;
    uint64_t pickd;
    uint8_t screen;
} processInfo;

typedef struct pipes_info{
	unsigned int id;
	unsigned int usage;
	unsigned int read_num_blocked;
	unsigned int read_blocked_pids[10];
	unsigned int write_num_blocked;
	unsigned int write_blocked_pids[10];
}pipes_info;

#endif
