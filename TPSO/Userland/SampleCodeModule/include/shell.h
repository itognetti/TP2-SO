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
#define MODULES 21
#define HALF_SECOND 50000000
#define ACTIVE_PROCESS 1
#define PAUSED_PROCESS 2
#define MEM_INFORMATION 3

#define ERR_PARAM_NO_NEED "This command does not require parameters, please try executing it without any.\n"
#define ERR_PIPE_UNSUPPORTED "Pipes are not supported by this program, please execute it without using pipes.\n"
#define ERR_MISSING_PARAMS "The program expects parameters.\n"
#define ERR_MISSING_PARAM "The program expects a parameter.\n"
#define ERR_MEMORY_ALLOC "Unable to allocate space for arguments!.\n"
#define ERR_INVALID_COMMAND "Unrecognized command. Use the help command to see available commands.\n"
#define ERR_INVALID_PID "The provided PID is invalid: No such process is currently running.\n"

#define INVALID_PID_CODE -1
#define EXEC_BG 0
#define EXEC_FG 1

#define PIPE "|"
#define MAX_CMD_WORDS 10
#define MIN_VALUE(a, b) ((a) <= (b) ? (a) : (b))

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
int handlePipeCommand(char ** words, unsigned int wordsCount);
void handleProcess(char ** words, unsigned int wordsCount);
int userCommandParser(char ** command, char readBuf[BUFFER_SIZE]);

void initShell();
void help();
void divZero();
void invOpCode();
void time();
void pong();
void infoReg();
void clear();
void testMemoryManager(char * argv[]);
void testProcesses(char *argv[]);
void testPriorities();
void testSync(char *argv[]);

void loop();
void cat();
void wc();
void filter();
void ps();
void kill(char ** args);
void nice(char ** args);
void block(char ** args);
void philo();
void mem();
#endif