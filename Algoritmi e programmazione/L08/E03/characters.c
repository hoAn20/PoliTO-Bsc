#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "characters.h"

char *getCode(character_t *character) {
    return &(character->code);
}
character_t READcharacter(FILE *f) {
    character_t new;

    if (f == stdin) printf("Inserisci il codice del personaggio: ");
    fscanf(f, "%s", new.code);
    if (f == stdin) printf("Inserisci il nome del personaggio: ");
    fscanf(f, "%s", new.name);
    if (f == stdin) printf("Inserisci la classe del personaggio: ");
    fscanf(f, "%s", new.class);
    if (f == stdin) printf("Inserisci la statistica hp del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[hp]);
    if (f == stdin) printf("Inserisci la statistica mp del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[mp]);
    if (f == stdin) printf("Inserisci la statistica atk del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[atk]);
    if (f == stdin) printf("Inserisci la statistica def del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[def]);
    if (f == stdin) printf("Inserisci la statistica mag del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[mag]);
    if (f == stdin) printf("Inserisci la statistica spr del personaggio: ");
    fscanf(f, "%d", &new.stats.stat[spr]);

    return new;
}int existingCode(characterTab_t TABch, char *code) {
    LINK x;

    for (x = TABch.listHead; x != NULL; x = x->nextCharacter) {
        if (strcmp(code, getCode(&(x->character))) == 0)
            return 1;
    }

    return 0;
}
LINK LISTcharacters_newNode(character_t new, LINK next) {
    LINK newNodeLink = (LINK)malloc(sizeof(NODE));

    newNodeLink->character = new;
    newNodeLink->nextCharacter = next;
    for (int i = 0; i < N_OBJECTS; i++)
        newNodeLink->character.equipment.objects_p[i] = NULL;

    return newNodeLink;
}
LINK LISTcharacters_search(characterTab_t TABch, char *key) {
    LINK x;

    for (x = TABch.listHead; strcmp(getCode(&(x->character)), key) != 0; x = x->nextCharacter);

    return x;
}
void LISTcharacters_insertInTail(characterTab_t *TABch_p, character_t new) {
    if (TABch_p->listLength == 0) {
        TABch_p->listHead = LISTcharacters_newNode(new, NULL);
        TABch_p->listTail = TABch_p->listHead;
        return;
    }
    TABch_p->listTail->nextCharacter = LISTcharacters_newNode(new, NULL);
    TABch_p->listTail = TABch_p->listTail->nextCharacter;
}
void LISTcharacters_print(characterTab_t TABch, FILE *f) {
    LINK x;

    for (x = TABch.listHead; x != NULL; x = x->nextCharacter)
        fprintf(f, "%s %s %s %d %d %d %d %d %d\n", x->character.code, x->character.name, x->character.class, x->character.stats.stat[hp], x->character.stats.stat[mp], x->character.stats.stat[atk], x->character.stats.stat[def], x->character.stats.stat[mag], x->character.stats.stat[spr]);
}
void LISTcharacters_deleteNode(characterTab_t *TABch, char *delCode) {
    LINK x, d;

    for (x = TABch->listHead; strcmp(delCode, getCode(&(x->nextCharacter->character))) != 0; x = x->nextCharacter);
    d = x->nextCharacter;
    x->nextCharacter = d->nextCharacter;
    free(d);
}