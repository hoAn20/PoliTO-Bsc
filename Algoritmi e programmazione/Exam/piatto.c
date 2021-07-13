#include "piatto.h"

Piatto PIATTOread(FILE *f) {
    Piatto p;
    fscanf(f, "%s %s %s %f", p.nome, p.portata, p.tipo, &p.costo);
    return p;
}
void PIATTOprint(Piatto p) {
    printf("%s %s %s %.2f\n", p.nome, p.portata, p.tipo, p.costo);
}
