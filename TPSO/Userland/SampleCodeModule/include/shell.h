#ifndef SHELL_H
#define SHELL_H

#include <libIO.h>
#include <colors.h>
#include <date.h>
#include <string.h>
#include <pong.h>
#include "loader.h"

#define BUFFER_SIZE 25
#define STDIN 1
#define STDOUT 1
#define MODULES 20
#define HALF_SECOND 500000000
#define ACTIVE_PROCESS 1
#define PAUSED_PROCESS 2
#define MEMINFO 3

#define IS_VOWEL(c) ((c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || \
                     c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'))

typedef struct{
    char * name;
    char * description;
    uint64_t function;
    int args;
    int pipe;
} modules;

extern void invalidOperationCode();

unsigned int validateProgram(char * string);
char ** createProgramParams(char ** words, unsigned int len);
int handlePipeCommand(char ** words, unsigned int amount_of_words);
void handleProcess(char ** words, unsigned int amount_of_words);
int userCommandParser(char ** command, char readBuf[BUFFER_SIZE]);

void initShell();
void help();
void divZero();
void invOpCode();
void time();
void pong();
void infoReg();
void clear();
void testMemoryManager();
void testProcesses(uint64_t argc, char *argv[]);
void testPriorities();

void loop();
void cat();
void wc();
void filter();
void ps();
void kill(char ** args);
void nice(char ** args);
void block(char ** args);
void philo();
void displayMemoryStatus();
#endif