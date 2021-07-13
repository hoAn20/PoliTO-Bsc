#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "inventory.h"

#define N_OBJECTS 8
#define CODE_LEN 6

typedef struct {
    int inUso;
    object_t *objects_p[N_OBJECTS];
} equipmentTab_t;
typedef struct {
    char code[CODE_LEN + 1];
    char name[STR_LEN + 1];
    char class[STR_LEN + 1];
    equipmentTab_t equipment;
    stat_t stats;
} character_t;
typedef struct node NODE, *LINK;
struct node {
    character_t character;
    LINK nextCharacter;
};
typedef struct {
    LINK listHead;
    LINK listTail;
    int listLength;
} characterTab_t;

char *getCode(character_t *character);
character_t READcharacter(FILE *f);
int existingCode(characterTab_t TABch, char *code);
LINK LISTcharacters_newNode(character_t new, LINK next);
LINK LISTcharacters_search(characterTab_t TABch, char *key);
void LISTcharacters_insertInTail(characterTab_t *TABch_p, character_t new);
void LISTcharacters_print(characterTab_t TABch, FILE *f);
void LISTcharacters_deleteNode(characterTab_t *TABch, char *delCode);

#endif
