#include <stdio.h>
#include <stdlib.h>

#include "element.h"

Element ELEMread(FILE *f) {
    Element elem;

    fscanf(f,
            "%s %d %d %d %d %d %f %d",
            elem.name, &elem.type, &elem.dirIn, &elem.dirOut, &elem.first, &elem.last, &elem.value, &elem.difficulty
            );

    return elem;
}
int ELEMcompatibility(Element pred, Element succ) {
    if (pred.dirOut == succ.dirIn)
        return 1;
    else
        return 0;
}