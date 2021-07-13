#include "menu.h"

void MENUprint(Menu m, Piatto *piatti, int P) {
    printf(" - MENU -\n");
    for (int i = 0; i < P; i++)
        PIATTOprint(piatti[m.scelte[i]]);
    printf("TOT: %.2f\n\n", m.costo);
}
int MENUcompare(Menu m1, Menu m2, int P) {
    if (m1.costo > m2.costo)
        return 1;
    if (m1.costo < m2.costo)
        return -1;
    for (int i = 0; i < P; i++) {
        if (m1.scelte[i] > m2.scelte[i])
            return 1;
        if (m1.scelte[i] < m2.scelte[i])
            return -1;
    }

    return 0;
}
