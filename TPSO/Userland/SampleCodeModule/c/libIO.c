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
    read(STDIN, &c, 1);
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
    while(((c = getChar()) != '\n' && c != -1) && bufferDim < bufferSize){
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

void invertedString(char * string, int len){
    char aux;
    for(int i = 0, j = len - 1; i < j; i++, j--){
        aux = string[i];
        string[i] = string[j];
        string[j] = aux;
    }
}

char * int64ToStringConverter(int64_t number){
    char * buff = (char *) alloc(21);
    if(buff == NULL){
        return NULL;
    }
    if(number == 0){
        buff[0] = '0';
        buff[1] = 0;
        return buff; 
    }
    int index = 0;
    while(number > 0){
        buff[index++] = number % 10 + '0';
        number /= 10;
    }
    invertedString(buff, index);
    buff[index] = 0;
    return buff;
}

static unsigned int seed = 12345; // Semilla inicial

int getRandomSign() {
    seed = (seed * 1103515245 + 12345) % 2147483648;     // Generación del número pseudoaleatorio
    return (seed % 2 == 0) ? 1 : -1;    // Devuelve 1 si es par, -1 si es impar
}