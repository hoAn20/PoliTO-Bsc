#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"

Date DATEfromString(char *dateString) {
    Date date;
    sscanf(dateString, "%d/%d/%d", &date.y, &date.m, &date.d);

    return date;
}
int DATEcompare(Date date1, Date date2) {   //ritorna valore un valore positivo
    if (date1.y == date2.y) {               //se date1 > date2, un valore negativo
        if (date1.m == date2.m)             //se date1 < date2 e 0 se sono uguali
            return date1.d - date2.d;
        else
            return date1.m - date2.m;
    } else
        return date1.y - date2.y;
}
Time TIMEfromString(char *timeString) {
    Time time;
    sscanf(timeString, "%d:%d", &time.h, &time.m);

    return time;
}
int DATEisBetween(Date date1, Date date, Date date2) {
    return (DATEcompare(date1, date) < 0 && DATEcompare(date, date2) < 0);
}
void DATEprint(Date date) {
    printf("%d/%d/%d\n", date.y, date.m, date. d);
}
