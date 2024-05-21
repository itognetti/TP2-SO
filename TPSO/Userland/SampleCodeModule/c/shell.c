#include <shell.h>

modules module[] = {
    {"help", "          -    Displays the module list", help},
    {"divzero", "       -    Throws Divide by Zero exception", divZero},
    {"invopcode", "     -    Throws Invalid Operation Code exception", invOpCode},
    {"time", "          -    Displays the systems current time", time},
    {"pong", "          -    Loads Pong game", pong},
    {"inforeg", "       -    Displays the registers state", infoReg},
    {"clear", "         -    Clears the screen", clear}
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
