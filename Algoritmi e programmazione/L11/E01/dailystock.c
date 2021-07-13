#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dailystock.h"

DailyStock DSTOCKnew(FILE *f) {
    DailyStock dStock;
    char date[MAX_DATE_LEN], time[MAX_TIME_LEN];
    float value;
    int nStock;
    fscanf(f, "%s %s %f %d", date, time, &value, &nStock);
    dStock.date = DATEfromString(date);
    dStock.time = TIMEfromString(time);
    dStock.value = value;
    dStock.valueSumWt = value * nStock;
    dStock.nStockSum = nStock;

    return dStock;
}
DailyStock DSTOCKsetNull() {
    DailyStock dStock;
    dStock.date = DATEfromString("0000/01/01");
    dStock.value = -1;
    dStock.valueSumWt = -1;
    dStock.nStockSum = -1;

    return dStock;
}
void DSTOCKupdate(DailyStock *dStock, float newValue, int nNewStock) {
    dStock->valueSumWt+= newValue * nNewStock;
    dStock->nStockSum+= nNewStock;
    dStock->value = dStock->valueSumWt / dStock->nStockSum;
}
