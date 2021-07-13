#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "inventory.h"

#define MAX_OBJ_EQUIPPED 8

typedef struct equipment *Equipment;

Equipment EQUIPnew();
void EQUIPfree(Equipment equip);
int EQUIPnObjectsEquipped(Equipment equip);
void EQUIPprint(Equipment equip, Inventory inv);
void EQUIPaddEquipment(Equipment equip, int index);
void EQUIPdelEquipment(Equipment equip, int index);
int EQUIPgetSize(Equipment equip);
Stat EQUIPgetModifiers(Equipment equip, Inventory inv);

#endif