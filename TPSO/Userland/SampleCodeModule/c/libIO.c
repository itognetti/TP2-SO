#include <libIO.h>

void printIn(char *string, int row, int col, int color){
    write(string, row, col, color);
}

void printf(char *string){
    printIn(string, -1, -1, DEFAULT_COLOR);
}

void println(char *string){
    printf(string);
    printf("\n");
}

void printColored(char *string, int color){
    printIn(string, -1, -1, color);
}

void printlnColored(char * string, int color){
    printColored(string, color);
    printf("\n");
}

char getChar(){
    char c;
    read(&c, 1);
    return c;
}

void putChar(char c) {
    char buffer[2];
    buffer[0] = c;
    buffer[1] = '\0';
    printf(buffer);
}

void deleteChar(){
    printf("\b");
}

void scanf(char *buffer, int bufferSize) {
    char c;
    int bufferDim = 0;
    while(((c = getChar()) != '\n') && bufferDim < bufferSize){
        if(c != 0){
            if(c == '\b'){
                if(bufferDim > 0){
                    buffer[--bufferDim] = 0;
                    deleteChar();
                }
            }else{
                buffer[bufferDim++] = c;
                putChar(c);
            }
        }
    }
    buffer[bufferDim] = '\0';
}

void printTime(date currentDate){
    printColored("Local time & date: ", PINK);
    char buffer[3] = {0};

    printf(dateConverter(currentDate->hour, buffer));
    printf(":");
    printf(dateConverter(currentDate->minutes, buffer));
    printf(":");
    printf(dateConverter(currentDate->seconds, buffer));
    printf(" ");
    printf(dateConverter(currentDate->day, buffer));
    printf("/");
    printf(dateConverter(currentDate->month, buffer));
    printf("/");
    println(dateConverter(currentDate->year, buffer));
}

char * int64ToStringConverter(int64_t number){
    char * str = (char *) malloc(21);
    if(str == NULL){
        return NULL;
    }
    // snprintf();
    return str;
}

static unsigned int seed = 12345; // Semilla inicial

int getRandomSign() {
    seed = (seed * 1103515245 + 12345) % 2147483648;     // Generación del número pseudoaleatorio
    return (seed % 2 == 0) ? 1 : -1;    // Devuelve 1 si es par, -1 si es impar
}