#ifndef DATE_H
#define DATE_H

#define BUFFER_SIZE 25
#define STDIN 0
#define STDOUT 1

typedef struct dateData {
    int seconds;
    int minutes;
    int hour;
    int day;
    int month;
    int year;
} dateData;

typedef struct dateData *date;

#endif