#include "character.h"

struct character {
    char code[CODE_LEN+1];
    char name[STR_LEN+1];
    char class[STR_LEN+1];
    Equipment equip;
    Stat base;
    Stat effective;
};

Character CHARACTERnew() {
    Character character;

    character = (Character)malloc(sizeof(*character));
    character->equip = EQUIPnew();

    return character;
}
void CHARACTERfree(Character character) {
    EQUIPfree(character->equip);
    free(character);
}
Character CHARACTERread(FILE *f) {
    Character character = CHARACTERnew();

    fscanf(f, "%s %s %s", character->code, character->name, character->class);
    character->base = STATread(f);
    character->effective = character->base;

    return character;
}
void CHARACTERprint(Character character) {
    printf("%s %s %s\n", character->code, character->name, character->class);
    STATprint(character->effective);
}
void CHARACTERaddEquipment(Character character, Inventory inv) {
    int i, choose;

    printf("Il personaggio con codice %s possiede:\n", character->code);
    EQUIPprint(character->equip, inv);
    printf("\n\nGli oggetti disponibili nell'inventario sono:\n");
    INVENTORYprint(inv);
    do {
        printf("Inserire l'indice dell'oggetto desiderato: ");
        scanf("%d", &choose);
        choose--;
    } while(choose < 0 || choose >= INVENTORYgetSize(inv));

    EQUIPaddEquipment(character->equip, choose);
}
void CHARACTERdelEquipment(Character character, Inventory inv) {
    int i, choose;

    printf("Il peronaggio con codice %s possiede:\n", character->code);
    EQUIPprint(character->equip, inv);
    do {
        printf("\n\nInserire l'indice dell'oggetto che si desidera rimuovere: ");
        scanf("%d", &choose);
        choose--;
    } while(choose < 0 || choose > EQUIPgetSize(character->equip));

    EQUIPdelEquipment(character->equip, choose);
}
char *CHARACTERgetCode(Character character) {
    return character->code;
}
Character CHARACTERnewFromKeyboard(char *code) {
    Character character;

    character = CHARACTERnew();
    strcpy(character->code, code);
    printf("Inserisci il nome: ");
    scanf("%s", character->name);
    printf("Inserisci la classe: ");
    scanf("%s", character->class);
    character->effective = character->base = STATinputFromKeyboard();

    return character;
}
void CHARACTERcalculateEffectiveStats(Character character, Inventory inv) {
    Stat modifiers;

    modifiers = EQUIPgetModifiers(character->equip, inv);
    character->effective = STATcalculator(character->base, modifiers);
}
Stat CHARACTERgetEffectiveStat(Character character) {
    return character->effective;
}