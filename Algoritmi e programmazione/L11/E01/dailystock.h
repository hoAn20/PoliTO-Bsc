#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datetime.h"

#ifndef DAILYSTOCK_H
#define DAILYSTOCK_H

#define MAX_STR_LEN 20

typedef struct {
    Date date;
    Time time;
    float value;
    float valueSumWt;
    int nStockSum;
} DailyStock;

DailyStock DSTOCKnew(FILE *f);
DailyStock DSTOCKsetNull();
void DSTOCKupdate(DailyStock *dStock, float newValue, int nNewStock);

#endif
