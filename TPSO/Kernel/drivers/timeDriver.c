// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <timeDriver.h>

#define UNI_MASK 0x0F
#define DEC_MASK 0XF0

void getDate(date currentDate){
    int seconds = getTimeFormat(getSeconds());
    int minutes = getTimeFormat(getMinutes());
    int hour = getTimeFormat(getHour());
    int day = getTimeFormat(getDay());
    int month = getTimeFormat(getMonth());
    int year = getTimeFormat(getYear());
    
    if (hour >= 3) {
        hour -= 3;
    } else {
        hour += 21; 
        day--; // Retroceder al día anterior
        if (day == 0) {
            month--; // Retroceder al mes anterior

            if (month == 0) {
                year--; // Retroceder al año anterior
                month = 12; // Establecer el mes a diciembre
            }
            // Obtener el último día del mes anterior
            day = getLastDayOfMonth(month, year);
        }
    }

    currentDate->seconds = seconds;
    currentDate->minutes = minutes;
    currentDate->hour = hour;
    currentDate->day = day;
    currentDate->month = month;
    currentDate->year = year;
}

int getLastDayOfMonth(int month, int year) {
    int lastDay;

    if (month == 2) {
        if (isLeapYear(year)) {
            lastDay = 29;
        } else {
            lastDay = 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        lastDay = 30;
    } else {
        lastDay = 31;
    }

    return lastDay;
}

int isLeapYear(int year) {
    if (year % 400 == 0) {
        return 1;
    } else if (year % 100 == 0) {
        return 0;
    } else if (year % 4 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int getTimeFormat(int num){
    return ((num & DEC_MASK) >> 4) * 10 + (num & UNI_MASK);
}