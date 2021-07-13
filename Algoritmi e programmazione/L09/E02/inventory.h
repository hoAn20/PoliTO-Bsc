#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

typedef struct inventory *Inventory;

Inventory INVENTORYnew();
void INVENTORYfree(Inventory inv);
void INVENTORYread(Inventory inv);
void INVENTORYprint(Inventory inv);
void INVENTORYprintObjectByIndex(Inventory inv, int index);
int INVENTORYgetSize(Inventory inv);
Object INVENTORYgetObjectByIndex(Inventory inv, int index);

#endif