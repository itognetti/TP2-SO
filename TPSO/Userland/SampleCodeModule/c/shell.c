#include <shell.h>

#define ERR_PARAM_NO_NEED "This command does not require parameters, please try executing it without any.\n"
#define ERR_PIPE_UNSUPPORTED "Pipes are not supported by this program, please execute it without using pipes.\n"
#define ERR_MISSING_PARAMS "The program expects %d parameters.\n"
#define ERR_MISSING_PARAM "The program expects a parameter.\n"
#define ERR_INVALID_PROGRAM "The program you are trying to run does not exist. Refer to the help command for help.\n"
#define ERR_MEMORY_ALLOC "Unable to allocate space for arguments!.\n"
#define ERR_INVALID_COMMAND "Unrecognized command. Use the help command to see available commands.\n"
#define ERR_INVALID_PID "The provided PID is invalid: No such process is currently running.\n"

#define INVALID_PID_CODE -1
#define EXEC_BG 0
#define EXEC_FG 1

#define PIPE "|"
#define MAX_CMD_WORDS 10
#define MIN_VALUE(a, b) ((a) <= (b) ? (a) : (b))

modules module[] = {
    {"help","              -    Displays the module list", (uint64_t)&help,0,0},
    {"divzero","           -    Throws Divide by Zero exception",(uint64_t) &divZero,0,0},
    {"invopcode","         -    Throws Invalid Operation Code exception",(uint64_t) &invOpCode,0,0},
    {"time","              -    Displays the systems current time",(uint64_t) &time,0,0},
    {"pong","              -    Loads Pong game",(uint64_t) &pong,0,0},
    {"inforeg","           -    Displays the registers state",(uint64_t) &infoReg,0,0},
    {"clear","             -    Clears the screen",(uint64_t) &clear,0,0},
    {"testMemoryManager"," -    Test memory manager",(uint64_t) &testMemoryManager ,0,0},
    {"testProcesses","     -    Test process management",(uint64_t) &testProcesses,1,0},
    {"testPriorities","    -    Test priorities",(uint64_t) &testPriorities,0,0},
    {"mem","               -    Prints the memory status",(uint64_t) &displayMemoryStatus,0,0},
    {"ps","                -    Shows every running process and its data",(uint64_t) &ps,0,0},
    {"loop","              -    Prints its id with a greeting every a specified number of seconds",(uint64_t) &loop,0,1},
    {"kill","              -    Kills a process given its id",(uint64_t) &kill,1,0},
    {"nice","              -    Changes the priority process",(uint64_t) &nice,2,0},
    {"block","             -    Changes the state of a process between blocked and ready given its id",(uint64_t) &block,1,0},    
    {"cat","               -    Prints the stdin as it is received",(uint64_t) &cat,0,1},
    {"wc","                -    Counts the number of lines in the input",(uint64_t) &wc,0,0},
    {"filter","            -    Filters the vowels from the input",(uint64_t) &filter,0,1},
    {"phylo","             -    Implements the dining philosophers problem",(uint64_t)&philo,0,0}
};

static char *starter = "$> ";
int firstInit = 1;

void nice(char **args) {
    if (!isNum(args[1]) && !isNum(args[2])) {
        printf("Invalid argument! Arguments must be numbers.\n");
        return;
    }
    unsigned int pid = _atoi(args[1]);
    int priorityDelta = _atoi(args[2]);
    niceProcess(pid, priorityDelta);
}

void block(char **args) {
    if (!isNum(args[1])) {
        printf("Invalid argument! Argument must be a number.\n");
        return;
    }
    uint64_t pid = _atoi(args[1]);
    pauseOrUnpauseProcess((unsigned int)pid);
    return;
}

static char *memoryInfoLabels[] = {"Allocated Bytes: ", "Free Bytes: ", "Allocated Blocks: "};

void displayMemoryStatus() {
    uint64_t memoryInfo[MEMINFO] = {0};
    memoryManStatus(memoryInfo);

    printf("Total Memory: ");

    char totalMemoryBuffer[BUFFER_SIZE] = {0};
    _strncpy(totalMemoryBuffer, int64ToStringConverter(memoryInfo[0] + memoryInfo[1]), BUFFER_SIZE);
    println(totalMemoryBuffer);

    for (int i = 0; i < MEMINFO; i++) {
        char infoBuffer[BUFFER_SIZE] = {0};
        printf(memoryInfoLabels[i]);
        _strncpy(infoBuffer, int64ToStringConverter(memoryInfo[i]), BUFFER_SIZE);
        println(infoBuffer);
    }
}

void initShell(){
    if(firstInit){
        println("Welcome! Enter help to display module list");
        firstInit = 0;
    }
    char * command[MAX_CMD_WORDS] = {0};
    char buffer[BUFFER_SIZE] = {0};
    while(1){
        printColored(starter, GREEN);
        scanf(buffer, BUFFER_SIZE);
        int commandWords = userCommandParser(command, buffer);
        println("");
        if(commandWords == 0)
          continue;

        if(handlePipeCommand(command,commandWords) == 0){
          handleProcess(command,commandWords);
        }
        _memset(buffer, 0, BUFFER_SIZE);
    } 
}

void help(){
    for(int i = 0; i < MODULES; i++){
        printf("             ");
        printColored(module[i].name, GREY);
        printlnColored(module[i].description, GREY);
    }
}

int userCommandParser(char **command, char buffer[BUFFER_SIZE]) {
    int i = 0, commandWords = 0;
	
	for(int postSpace = 1; commandWords < MAX_CMD_WORDS && buffer[i] != '\n' && buffer[i] != 0; i++) {
        if(buffer[i]==-1){
            buffer[i]=0;
            return commandWords;
        }
        if(buffer[i] == ' ') {
          postSpace = 1;
          buffer[i] = 0;
        } else if(postSpace) {
          command[commandWords++] = buffer + i; 
          postSpace = 0;
        }
	}

  buffer[i] = 0;
  return commandWords;
}

unsigned int validateProgram(char * string){
    for(int i = 0; i < MODULES; i++){
        if(strcmp(string, module[i].name) == 0){
            return i;
        }
    }
    return -1;
}

char **createProgramParams(char **words, unsigned int len) {
    void * ptr = (void*) alloc((2 + len) * sizeof(char *)); // + 1 for name, + 1 por null termination

    if(ptr == NULL){
        printf(ERR_MEMORY_ALLOC);   
        return NULL;
    }

    char ** params = (char **) ptr;

    void * param;
    int paramLen;

    int i=0;
    for(; i<=len + 1; i++){
        paramLen = strlen(words[i]) + 1;
        param = (void*) alloc(paramLen);

         if(param == NULL){
            printf(ERR_MEMORY_ALLOC);     
            return NULL;
        }

        char * param2 = (char *) param;

        _strncpy(param2, words[i], paramLen);
        params[i] = param2;
    }
    params[i] = NULL;
    return params;
}

int handlePipeCommand(char **words, unsigned int amount_of_words) {
    if (amount_of_words != 3 || strcmp(PIPE, words[1]) != 0) {
        return 0; 
    }

    unsigned int p1 = validateProgram(words[0]);
    unsigned int p2 = validateProgram(words[2]);
  
    if (p1 == INVALID_PID_CODE || p2 == INVALID_PID_CODE) {
        printf(ERR_INVALID_COMMAND);
        return 1;
    }
    
    if (!module[p1].pipe) {
        printf(ERR_PIPE_UNSUPPORTED);
        return 1;
    }
    
    int pipe_id = registerPipeAvailable();

    if (pipe_id <= 0) {
        printf("Error creating pipe!");
        return 1;
    }

    registerChildProcess(module[p1].function, STDIN, pipe_id, createProgramParams(words, 0));
    registerChildProcess(module[p2].function, pipe_id, EXEC_FG, createProgramParams(words, 0));

    waitChildren();

    destroyPipe(pipe_id);

    return 2; 
}

void handleProcess(char ** words, unsigned int amount_of_words){
    unsigned int program_pos = validateProgram(words[0]);

    if(program_pos == -1){
        printf(ERR_INVALID_COMMAND);
        return;
    }
    if(amount_of_words - 1 < module[program_pos].args){
        printf(module[program_pos].args > 1 ? ERR_MISSING_PARAMS : ERR_MISSING_PARAM);
        return;
    }

    int i;
    for(i=module[program_pos].args + 1; i < amount_of_words; i++){
        if(strcmp("&", words[i]) == 0){ 
            registerProcess(module[program_pos].function, STDIN, BACKGROUND, createProgramParams(words, MIN_VALUE(i-1,module[program_pos].args))); //Run on Background
            return; 
        }
    }
    registerChildProcess(module[program_pos].function, STDIN, EXEC_FG, createProgramParams(words, MIN_VALUE(amount_of_words-1, module[program_pos].args))); 
    waitChildren();
}

void divZero(){
    int a = 1 /0 ;
}

void invOpCode(){
    invalidOperationCode();
}

void time(){
    dateData currentDate;
    getTime(&currentDate);
    printTime(&currentDate);
}

void pong(){
    menuPong();
    clearScreen();
}

void infoReg(){
    getRegisters();
}

void clear(){
    clearScreen();
}

void loop(){
	int pid = getPid();
	while(1){
		for(int i = 0; i < HALF_SECOND ; i++);
		printf("Process ID:");
        println(int64ToStringConverter(pid));
	}	
}

void cat(){
    char buff[BUFFER_SIZE] = {0};
    scanf(buff, BUFFER_SIZE);
    println(buff);
}

void wc(){
    char c;
    int linesCount = 0;
    while((c = getChar()) != -1){
        if(c == '\n'){
            linesCount++;
        }
        putChar(c);
    }
    printf("Total input lines: ");
    println(int64ToStringConverter(linesCount));
}

void filter(){
    int j = 0;
    char buff[BUFFER_SIZE] = {0};
    char output[BUFFER_SIZE] = {0};
    char c;

    output[j++] = '\n';
    scanf(buff, BUFFER_SIZE);
    for(int i = 0; buff[i] == '\0' || i < BUFFER_SIZE; i++){
        c = buff[i];
        if(!IS_VOWEL(c)){
            output[j++] = c;
        }
    }
    println(output);
}

void ps(){
	processInfo * info = (void *) alloc(20 * sizeof(processInfo)); 

	if(info == NULL) {
		printf("No more space\n");
		return;
	}

	uint64_t amount = getProcessInfo(info);

	for(int i = 0; i < amount; i++){
		printf("Name: ");
        printf(info[i].processName);
        printf("\t| PID: ");
        printf(int64ToStringConverter(info[i].processId));
        printf("\t| State: ");

        switch(info[i].processState){
            case ACTIVE_PROCESS: 
                printf("Active\t| "); break;
            case PAUSED_PROCESS:
                printf("Paused\t| "); break;
            default:
                printf("Blocked\t| "); break;
        }

        printf("Priority: ");
        printf(int64ToStringConverter(info[i].processPriority));
        printf("\t| Stack: ");
        printf(int64ToStringConverter(info[i].processStack));
        printf("\t| RSP: ");
        printf(int64ToStringConverter(info[i].processRsp));
        printf("\t| Screen: ");

        switch(info[i].processScreen) {
            case BACKGROUND:
                printf("Background\n"); break;
            case STDOUT:
                printf("STDOUT\n"); break;
            default:
                printf("Pipe\n"); break;
        }
	}
	freeMem((void*)info);
}

void kill(char ** args){
  if(!isNum(args[1])) { 
    printf("Kill's argument must be number (process id).\n");
    return;
  }

  uint64_t pid = _atoi(args[1]);

  if (killProcess(pid) == INVALID_PID_CODE){
    printf(ERR_INVALID_PID);
  }

  return;
}