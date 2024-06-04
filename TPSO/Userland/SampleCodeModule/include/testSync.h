#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include <stdint.h>
#include <syscalls.h>
#include <testUtils.h>
#include <shell.h>
#include <libIO.h>


#define SEM_ID 2024
#define TOTAL_PAIR_PROCESSES 2
#define MAX 400

int64_t global; // shared memory

#endif