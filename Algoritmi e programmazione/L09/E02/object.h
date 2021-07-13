#ifndef OBJECT_H
#define OBJECT_H

#include "stat.h"

#define STR_LEN 50

typedef struct {
    char name[STR_LEN+1];
    char type[STR_LEN+1];
    Stat modifiers;
} Object;

Object OBJread(FILE *f);
void OBJprint(Object obj, int mode);

#endif