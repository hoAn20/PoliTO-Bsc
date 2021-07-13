#include "equipment.h"

struct equipment {
    int indexObjectEquipped[MAX_OBJ_EQUIPPED];
    int inUse;
};

Equipment EQUIPnew() {
    Equipment equip;

    equip = (Equipment)malloc(sizeof(*equip));
    equip->inUse = 0;

    return equip;
}
void EQUIPfree(Equipment equip) {
    free(equip);
}
int EQUIPnObjectsEquipped(Equipment equip) {
    return equip->inUse;
}
void EQUIPprint(Equipment equip, Inventory inv) {
    int i;

    for (i = 0; i < equip->inUse; i++) {
        printf("%d) ", (i+1));
        INVENTORYprintObjectByIndex(inv, equip->indexObjectEquipped[i]);
    }
}
void EQUIPaddEquipment(Equipment equip, int index) {
    equip->indexObjectEquipped[equip->inUse] = index;
    equip->inUse++;
}
void EQUIPdelEquipment(Equipment equip, int index) {
    int i;

    for (i = index; i < equip->inUse; i++)
        equip->indexObjectEquipped[i] = equip->indexObjectEquipped[i];
    equip->inUse--;
}
int EQUIPgetSize(Equipment equip) {
    return equip->inUse;
}
Stat EQUIPgetModifiers(Equipment equip, Inventory inv) {
    int i;
    Stat effectiveModifiers = STATsetToZero();

    for (i = 0; i < equip->inUse; i++) {
        effectiveModifiers = STATcalculator(effectiveModifiers, INVENTORYgetObjectByIndex(inv, equip->indexObjectEquipped[i]).modifiers);
    }

    return effectiveModifiers;
}