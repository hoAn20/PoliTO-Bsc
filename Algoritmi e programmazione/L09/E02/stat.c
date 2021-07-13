#include "stat.h"

Stat STATread(FILE *f) {
    Stat stat;

    fscanf(f, "%d %d %d %d %d %d",
            &stat.hp,
            &stat.mp,
            &stat.atk,
            &stat.def,
            &stat.mag,
            &stat.spr
            );

    return stat;
}
Stat STATcalculator(Stat base, Stat mod) {
    Stat effective;

    effective.hp = (base.hp + mod.hp >= STAT_MIN_VALUE)?(base.hp + mod.hp):STAT_MIN_VALUE;
    effective.mp = (base.mp + mod.mp >= STAT_MIN_VALUE)?(base.mp + mod.mp):STAT_MIN_VALUE;
    effective.atk = (base.atk + mod.atk >= STAT_MIN_VALUE)?(base.atk + mod.atk):STAT_MIN_VALUE;
    effective.def = (base.def + mod.def >= STAT_MIN_VALUE)?(base.def + mod.def):STAT_MIN_VALUE;
    effective.mag = (base.mag + mod.mag >= STAT_MIN_VALUE)?(base.mag + mod.mag):STAT_MIN_VALUE;
    effective.spr = (base.spr + mod.spr >= STAT_MIN_VALUE)?(base.spr + mod.spr):STAT_MIN_VALUE;

    return effective;
}
void STATprint(Stat stat) {
    printf("HP: %d\n"
           "MP: %d\n"
           "ATK: %d\n"
           "DEF: %d\n"
           "MAG: %d\n"
           "SPR: %d\n",
           stat.hp, stat.mp, stat.atk, stat.def, stat.mag, stat.spr
           );
}
Stat STATinputFromKeyboard() {
    Stat stat;

    printf("Inserisci HP: ");
    scanf("%d", &stat.hp);
    printf("Inserisci MP: ");
    scanf("%d", &stat.mp);
    printf("Inserisci ATK: ");
    scanf("%d", &stat.atk);
    printf("Inserisci DEF: ");
    scanf("%d", &stat.def);
    printf("Inserisci MAG: ");
    scanf("%d", &stat.mag);
    printf("Inserisci SPR: ");
    scanf("%d", &stat.spr);

    return stat;
}
Stat STATsetToZero() {
    Stat stat;

    stat.hp = stat.mp = stat.atk = stat.def = stat.mag = stat.spr = 0;

    return stat;
}