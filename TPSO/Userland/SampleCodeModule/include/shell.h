#ifndef SHELL_H
#define SHELL_H

#include <libIO.h>
#include <colors.h>
#include <date.h>
#include <string.h>
#include <pong.h>

#define BUFFER_SIZE 25
#define STDIN 1
#define STDOUT 1
#define MODULES 9

typedef struct{
    char * name;
    char * description;
    void (*function)();
} modules;

extern void invalidOperationCode();

void * memset(void * destiny, int32_t c, uint64_t length);

void initShell();
void callModule(char *buffer);
void help();
void divZero();
void invOpCode();
void time();
void pong();
void infoReg();
void clear();
void testMm();
uint64_t testProcesses(uint64_t argc, char *argv[]);

// TODO add functions (cat, wc, etc.)

#endif