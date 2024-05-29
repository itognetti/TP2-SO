#include <string.h>

int strlen(char *string){
    int count = 0;
    while (string[count] != 0) {
        count++;
    }
    return count;
}

int strcmp(const char *str1, const char *str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return (str1[i] - str2[i]);
        }
        i++;
    }
    return (str1[i] - str2[i]);
}
int strncpy(char * dest, const char * src, unsigned int n){
    int i=0;
    for(; src[i]!=0 && i<n; i++){
        dest[i] = src[i];
    }
    dest[i] = 0;
    return i;
}
char * reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    return str;
}

char * dateConverter(int num, char* str) {
    int i = 0;

    // Genero la cadena de caracteres en reversa
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num != 0);
    
    // Agregamos un 0 si el numero es de 1 digito
    if (i == 1){
        str[i++] = '0';
    }

    str[i] = '\0';
    // Invierto la cadena para obtener el resultado
    return reverse(str, i);
}

char * itoa(int num, char* str) {
    int i = 0;

    // Genero la cadena de caracteres en reversa
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num != 0);
    
    str[i] = '\0';
    // Invierto la cadena para obtener el resultado
    return reverse(str, i);
}

