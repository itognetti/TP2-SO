#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H

typedef struct dateData{
    int seconds;
    int minutes;
    int hour;
    int day;
    int month;
    int year;
} dateData;

typedef struct dateData *date;

extern int getSeconds();
extern int getMinutes();
extern int getHour();
extern int getDay();
extern int getMonth();
extern int  getYear();

void getDate(date currentDate);
int isLeapYear(int year);
int getLastDayOfMonth(int month, int year);
int getTimeFormat(int num);

#endif