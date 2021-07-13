#include "object.h"

Object OBJread(FILE *f) {
    Object obj;

    fscanf(f, "%s %s ", obj.name, obj.type);
    obj.modifiers = STATread(f);

    return obj;
}
Stat OBJgetStat(Object obj) {
    return obj.modifiers;
}
void OBJprint(Object obj, int mode) {
    if (mode == 1)
        printf("%s %s\n", obj.name, obj.type);
    else {
        printf("%s %s:\n", obj.name, obj.type);
        STATprint(obj.modifiers);
    }
}