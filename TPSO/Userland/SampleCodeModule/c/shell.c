#include <shell.h>

modules module[] = {
    {"help", "              -    Displays the module list", help},
    {"divzero", "           -    Throws Divide by Zero exception", divZero},
    {"invopcode", "         -    Throws Invalid Operation Code exception", invOpCode},
    {"time", "              -    Displays the systems current time", time},
    {"pong", "              -    Loads Pong game", pong},
    {"inforeg", "           -    Displays the registers state", infoReg},
    {"clear", "             -    Clears the screen", clear},
    {"testMemoryManager", " -    Test memory manager", testMemoryManager},
    {"testProcess", "       -    Test process management", testProcesses},
    {"testPriorities", "    -    Test priorities", testPriorities},
    {"loop", "              -    Prints its process id with a greeting every certain amount of seconds", loop},
    {"cat", "               -    Prints the stdin as it receives it", cat},
    {"wc", "                -    Counts the number of lines in the input", wc},
    {"filter", "            -    Filters the vowels from the input", filter}
};

static char *starter = "$> ";
int firstInit = 1;

void initShell(){
    if(firstInit){
        println("Welcome! Enter help to display module list");
        firstInit = 0;
    }
    char buffer[BUFFER_SIZE] = {0};
    while(1){
        printColored(starter, GREEN);
        scanf(buffer, BUFFER_SIZE);
        callModule(buffer);
        memset(buffer, 0, BUFFER_SIZE);
    } 
}

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

void help(){
    for(int i = 0; i < MODULES; i++){
        printf("             ");
        printColored(module[i].name, GREY);
        printlnColored(module[i].description, GREY);
    }
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
    printf(buff);
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
    printf(int64ToStringConverter(linesCount));
}

void filter(){
    int i, j = 0;
    char buff[BUFFER_SIZE] = {0};
    char output[BUFFER_SIZE] = {0};
    char c;

    output[j++] = '\n';
    scanf(buff, BUFFER_SIZE);
    for(int i = 0; buff[i] == 0 || i < BUFFER_SIZE; i++){
        c = buff[i];
        if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
            output[j++] = c;
        }
    }
    println(output);
}
