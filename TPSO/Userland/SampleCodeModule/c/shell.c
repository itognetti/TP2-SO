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
    {"help", "            -    Displays the module list", (uint64_t)&help,0,0},
    {"divzero", "         -    Throws Divide by Zero exception",(uint64_t) &divZero,0,0},
    {"invopcode", "       -    Throws Invalid Operation Code exception",(uint64_t) &invOpCode,0,0},
    {"time", "            -    Displays the systems current time",(uint64_t) &time,0,0},
    {"pong", "            -    Loads Pong game",(uint64_t) &pong,0,0},
    {"inforeg", "         -    Displays the registers state",(uint64_t) &infoReg,0,0},
    {"clear", "           -    Clears the screen",(uint64_t) &clear,0,0},
    {"testMemoryManager","-    Test memory manager",(uint64_t) &testMemoryManager ,0,0},
    {"testProcesses","    -    Test process management",(uint64_t) &testProcesses,1,0},
    {"testPriorities","   -    Test priorities",(uint64_t) &testPriorities,0,0},
    {"cat","              -    Writes in console what has been read",(uint64_t) &cat,0,1},
    {"loop","             -    Loops while printing the process id every half a second",(uint64_t) &loop,0,1},
    {"wc","               -    Counts the lines in what has been written in screen",(uint64_t) &wc,0,0},
    {"filter","           -    Filters what has been written and only shows consonants",(uint64_t) &filter,0,1}
};

static char *starter = "$> ";
int firstInit = 1;

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
        int commandWords = parseCommand(command, buffer);
        println("");
        if(commandWords == 0)
          continue;

        if(piped_process_handle(command,commandWords) == 0){
          single_process_handle(command,commandWords);
        }
        memset(buffer, 0, BUFFER_SIZE);
    } 
}
/*
void callModule(char *buffer){
    println("");
    for(int i = 0; i < MODULES; i++){
        if(strcmp(buffer, module[i].name) == 0){
            module[i].function();
            return;
        }
    }
    printf(buffer);
    println(": command not found, please enter 'help' for module list");
}
*/

void help(){
    for(int i = 0; i < MODULES; i++){
        printf("             ");
        printColored(module[i].name, GREY);
        printlnColored(module[i].description, GREY);
    }
}

int userCommandParser(char **command, char buffer[BUFFER_SIZE], int maxCommandWords) {
    int commandWords = 0;
    int postSpace = 1; 
    int index = 0;
    while (commandWords < maxCommandWords && buffer[index] != '\n' && buffer[index] != '\0') {
        if (isspace((unsigned char)buffer[index])) { 
            postSpace = 1;
            buffer[index] = '\0'; 
        } else if (postSpace) { 
            command[commandWords++] = buffer + index; 
            postSpace = 0;
        }
        index++;
    }
    buffer[index] = '\0'; 
    return commandWords;
}
unsigned int validateProgram(char * string){
    for(int i = 0; i < MODULES; i++){
        if(strcmp(string, module[i].name)==0){
            return i;
        }
    }
    return -1;
}


char **createProgramParams(char **words, unsigned int len) {
    size_t total_len = 0;
    for (int i = 0; i < len; i++) {
        total_len += strlen(words[i]) + 1; 
    }

    void *ptr = alloc((len + 1) * sizeof(char *) + total_len);
    if (ptr == NULL) {
        printf(ERR_MEMORY_ALLOC);
        return NULL;
    }

    char *param_mem = (char *)ptr + (len + 1) * sizeof(char *);

    char **params = (char **)ptr;
    for (int i = 0; i < len; i++) {
        params[i] = param_mem;
        strcpy(param_mem, words[i]); 
        param_mem += strlen(words[i]) + 1; 
    }
    params[len] = NULL; 

    return params;
}

int handlePipeCommand(char **words, unsigned int amount_of_words) {
    if (amount_of_words != 3 || strcmp(PIPE, words[1]) != 0) {
        return 0; 
    }

    unsigned int p1 = check_valid_program(words[0]);
    unsigned int p2 = check_valid_program(words[2]);
  
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

    char **params = make_params(words, 0);
    if (params == NULL) {
        return 1;
    }

    registerChildProcess(module[p1].function, STDIN, pipe_id, params);
    registerChildProcess(module[p2].function, pipe_id, EXEC_FG, params);

    waitChildren();

    destroyPipe(pipe_id);

    return 2; 
}
void  handleProcess(char ** words, unsigned int amount_of_words){
    unsigned int program_pos = check_valid_program(words[0]);

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
            registerChildProcess(module[program_pos].function, STDIN, BACKGROUND, make_params(words, MIN(i-1,module[program_pos].args))); //Run on Background
            return; 
        }
    }
    registerChildProcess(module[program_pos].function, STDIN, EXEC_FG, make_params(words, MIN(amount_of_words-1, module[program_pos].args))); 
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
