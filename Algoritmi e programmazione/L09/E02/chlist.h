#ifndef CHLIST_H
#define CHLIST_H

#include "character.h"

typedef struct chlist *ChList;

ChList CHLISTnew();
void CHLISTfree(ChList chlist);
void CHLISTreadFromFile(ChList chlist);
void CHLISTprint(ChList chlist);
void CHLISTinsertInTailCharacter(ChList chlist, Character character);
void CHLISTremoveCharacter(ChList chlist, char *code);
Character CHLISTsearchByCode(ChList chlist, char *code);
void CHLISTaddEquipmentToCharacter(ChList chlist, Inventory inv);
void CHLISTdelEquipmentToCharacter(ChList chlist, Inventory inv);

#endif