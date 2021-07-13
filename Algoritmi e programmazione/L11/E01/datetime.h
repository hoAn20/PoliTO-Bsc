#ifndef DATETIME_H
#define DATETIME_H

#define MAX_DATE_LEN 11
#define MAX_TIME_LEN 6

typedef struct {
    int h;
    int m;
} Time;
typedef struct {
    int y;
    int m;
    int d;
} Date;

Date DATEfromString(char *dateString);
int DATEcompare(Date date1, Date date2);
Time TIMEfromString(char *timeString);
int DATEisBetween(Date date1, Date date, Date date2);
void DATEprint(Date date);

#endif
