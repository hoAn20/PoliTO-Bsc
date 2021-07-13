#ifndef STAT_H
#define STAT_H

#include <stdio.h>
#include <stdlib.h>

#define STAT_MIN_VALUE 1

typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} Stat;

Stat STATread(FILE *f);
Stat STATcalculator(Stat base, Stat mod);
void STATprint(Stat stat);
Stat STATinputFromKeyboard();
Stat STATsetToZero();

#endif