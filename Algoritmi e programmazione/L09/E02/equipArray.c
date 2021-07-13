#include <stdlib.h>
#include "equipArray.h"

equipArray_t *NEWequipArray() {
    equipArray_t *newEquipArray;

    newEquipArray = malloc(sizeof(equipArray_t));

    return newEquipArray;
}
void DELequipArray(equipArray_t *equip) {
    free(equip);
}