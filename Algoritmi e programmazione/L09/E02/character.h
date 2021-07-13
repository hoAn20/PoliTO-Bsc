#ifndef CHARACTER_H
#define CHARACTER_H

#include "stat.h"
#include "inventory.h"
#include "equipment.h"

#define CODE_LEN 6

typedef struct character *Character;

Character CHARACTERnew();
void CHARACTERfree(Character character);
Character CHARACTERread(FILE *f);
void CHARACTERprint(Character character);
void CHARACTERaddEquipment(Character character, Inventory inv);
void CHARACTERdelEquipment(Character character, Inventory inv);
char *CHARACTERgetCode(Character character);
Character CHARACTERnewFromKeyboard(char *code);
void CHARACTERcalculateEffectiveStats(Character character, Inventory inv);
Stat CHARACTERgetEffectiveStat(Character character);

#endif