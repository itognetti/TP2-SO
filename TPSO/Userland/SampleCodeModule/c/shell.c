#include <shell.h>

modules module[] = {
    {"help","              -    Displays the module list", (uint64_t)&help,0,0},
    {"divzero","           -    Throws Divide by Zero exception",(uint64_t) &divZero,0,0},
    {"invopcode","         -    Throws Invalid Operation Code exception",(uint64_t) &invOpCode,0,0},
    {"time","              -    Displays the systems current time",(uint64_t) &time,0,0},
    {"pong","              -    Loads Pong game",(uint64_t) &pong,0,0},
    {"inforeg","           -    Displays the registers state",(uint64_t) &infoReg,0,0},
    {"clear","             -    Clears the screen",(uint64_t) &clear,0,0},
    {"testMM","            -    Test memory manager",(uint64_t) &testMemoryManager ,1,0},
    {"testProcesses","     -    Test process management",(uint64_t) &testProcesses,1,0},
    {"testPriority","      -    Test priorities",(uint64_t) &testPriorities,0,0},
    {"testSynchro","       -    Test synchronization",(uint64_t) &testSync, 3, 0},
    {"mem","               -    Prints the memory status",(uint64_t) &mem,0,0},
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
int firstInitialization = 1;
static char *memoryInfoStats[] = {"Allocated Bytes: ", "Free Bytes: ", "Allocated Blocks: "};

void initShell(){
    if(firstInitialization){
        println("Welcome! Enter help to display module list");
        firstInitialization = 0;
    }
    char * inputCommand[MAX_CMD_WORDS] = {0};
    char buffer[BUFFER_SIZE] = {0};
    while(1){
        printColored(starter, GREEN);
        scanf(buffer, BUFFER_SIZE);
        int commandWords = userCommandParser(inputCommand, buffer);
        println("");
        if(commandWords == 0)
          continue;

        if(handlePipeCommand(inputCommand,commandWords) == 0){
          handleProcess(inputCommand,commandWords);
        }
        _memset(buffer, 0, BUFFER_SIZE);
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
    void * ptr = (void*) alloc((2 + len) * sizeof(char *)); 

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

int handlePipeCommand(char **words, unsigned int wordsCount) {
    if (wordsCount != 3 || strcmp(PIPE, words[1]) != 0) {
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

void handleProcess(char ** words, unsigned int wordsCount){
    unsigned int program_pos = validateProgram(words[0]);

    if(program_pos == -1){
        printf(ERR_INVALID_COMMAND);
        return;
    }
    if(wordsCount - 1 < module[program_pos].args){
        printf(module[program_pos].args > 1 ? ERR_MISSING_PARAMS : ERR_MISSING_PARAM);
        return;
    }

    int i;
    for(i = module[program_pos].args + 1; i < wordsCount; i++){
        if(strcmp("&", words[i]) == 0){ 
            registerProcess(module[program_pos].function, STDIN, EXEC_BG, createProgramParams(words, MIN_VALUE(i - 1, module[program_pos].args))); 
            return; 
        }
    }
    registerChildProcess(module[program_pos].function, STDIN, EXEC_FG, createProgramParams(words, MIN_VALUE(wordsCount - 1, module[program_pos].args))); 
    waitChildren();
}

void help(){
    for(int i = 0; i < MODULES; i++){
        printf("             ");
        printColored(module[i].name, WHITE);
        printlnColored(module[i].description, WHITE);
    }
}

void divZero(){
    int a = 1 / 0;
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

void mem() {
    uint64_t memoryInfo[MEM_INFORMATION] = {0};
    memoryManStatus(memoryInfo);

    printf("Total Memory: ");

    char totalMemoryBuffer[BUFFER_SIZE] = {0};
    _strncpy(totalMemoryBuffer, int64ToStringConverter(memoryInfo[0] + memoryInfo[1]), BUFFER_SIZE);
    println(totalMemoryBuffer);

    for (int i = 0; i < MEM_INFORMATION; i++) {
        char infoBuffer[BUFFER_SIZE] = {0};
        printf(memoryInfoStats[i]);
        _strncpy(infoBuffer, int64ToStringConverter(memoryInfo[i]), BUFFER_SIZE);
        println(infoBuffer);
    }
}

void ps(){
	processInfo * info = (void *) alloc(20 * sizeof(processInfo)); 

	if(info == NULL) {
		printf("No more space\n");
		return;
	}

    printlnColored("NAME   PID   STATE   PRIORITY   STACK   RSP   EXECUTION", GREEN);

	uint64_t amount = getProcessInfo(info);

	for(int i = 0; i < amount; i++){
        printf(info[i].processName);
        printf("\t| ");
        printf(int64ToStringConverter(info[i].processId));
        printf("\t| ");

        switch(info[i].processState){
            case ACTIVE_PROCESS: 
                printf("Active"); break;
            case PAUSED_PROCESS:
                printf("Paused"); break;
            default:
                printf("Blocked"); break;
        }
        printf("\t| ");
        printf(int64ToStringConverter(info[i].processPriority));
        printf("\t| ");
        printf(int64ToStringConverter(info[i].processStack));
        printf("\t| ");
        printf(int64ToStringConverter(info[i].processRsp));
        printf("\t| ");

        switch(info[i].processScreen) {
            case EXEC_BG:
                printf("BACKGROUND\n"); break;
            case EXEC_FG:
                printf("STDOUT\n"); break;
            default:
                printf("PIPE\n"); break;
        }
	}
	freeMem((void*)info);
}

void loop(){
    println("Executing please wait");
	int pid = getPid();
	while(1){
		for(int i = 0; i < HALF_SECOND ; i++);
		printf("Process id: ");
        println(int64ToStringConverter(pid));
	}	
}

void kill(char ** args){
  if(!isNum(args[1])) { 
    printf("Argument must be process id.\n");
    return;
  }

  uint64_t pid = _atoi(args[1]);

  if (killProcess(pid) == INVALID_PID_CODE){
    printf(ERR_INVALID_PID);
  }

  return;
}

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
    printColored("Total lines: ", GREEN);
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