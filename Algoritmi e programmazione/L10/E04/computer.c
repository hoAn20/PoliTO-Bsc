#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "computer.h"

Computer COMPnew(char *name, char *net) {
    Computer cp;
    strcpy(cp.name, name);
    strcpy(cp.net, net);

    return cp;
}
Computer COMPsetNull() {
    Computer cp;
    strcpy(cp.name, "");
    strcpy(cp.net, "");

    return cp;
}
