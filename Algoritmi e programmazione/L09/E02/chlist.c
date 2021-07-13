#include "chlist.h"

typedef struct chnode CHNODE, *CHLINK;
struct chnode {
    Character character;
    CHLINK next;
};
struct chlist {
    CHLINK head;
    CHLINK tail;
    int listLength;
};

CHLINK CHLISTnewNode(Character character, CHLINK next);

ChList CHLISTnew() {
    ChList chlist;

    chlist = (ChList)malloc(sizeof(*chlist));
    chlist->head = NULL;
    chlist->listLength = 0;

    return chlist;
}
void CHLISTfree(ChList chlist) {
    free(chlist->head);
    free(chlist->tail);
    free(chlist);
}
void CHLISTfreeNode(CHLINK chlink) {
    CHARACTERfree(chlink->character);
    free(chlink);
}
void CHLISTreadFromFile(ChList chlist) {
    FILE *f;
    char fileName[STR_LEN+1];

    do {
        printf("Inserirsci il nome del file da cui leggere la lista di personaggi: ");
        scanf("%s", fileName);
        f = fopen(fileName, "r");
    } while(f == NULL);

    while(!feof(f)) {
        CHLISTinsertInTailCharacter(chlist, CHARACTERread(f));
        chlist->listLength++;
    }
}
void CHLISTprint(ChList chlist) {
    CHLINK x;
    for (x = chlist->head; x!= NULL; x = x->next)
        CHARACTERprint(x->character);
}
CHLINK CHLISTnewNode(Character character, CHLINK next) {
    CHLINK new = (CHLINK)malloc(sizeof(CHNODE));

    new->character = character;
    new->next = next;

    return new;
}
void CHLISTinsertInTailCharacter(ChList chlist, Character character) {
    CHLINK tmp;
    if (chlist->head == NULL) {
        chlist->head = CHLISTnewNode(character, NULL);
        chlist->tail = chlist->head;
    } else {
        tmp = chlist->tail;
        tmp->next = CHLISTnewNode(character, NULL);
        chlist->tail = tmp->next;
    }
}
void CHLISTremoveCharacter(ChList chlist, char *code) {
    CHLINK x, tmp;
    if (strcmp(CHARACTERgetCode(chlist->head->character), code) == 0) {
        tmp = chlist->head;
        chlist->head = tmp->next;
        CHLISTfreeNode(tmp);
    } else {
        for (x = chlist->head; strcmp(CHARACTERgetCode(x->next->character), code)!= 0; x = x->next);
        tmp = x->next;
        x->next = tmp->next;
        CHLISTfreeNode(tmp);
    }
}
Character CHLISTsearchByCode(ChList chlist, char *code) {
    CHLINK x;

    for (x = chlist->head; x != NULL; x = x->next) {
        if (strcmp(CHARACTERgetCode(x->character), code) == 0)
            return x->character;
    }

    return NULL;
}
void CHLISTaddEquipmentToCharacter(ChList chlist, Inventory inv) {
    char code[CODE_LEN+1];
    Character character;

    do {
        printf("Inserisci il codice del personaggio: ");
        scanf("%s", code);
        character = CHLISTsearchByCode(chlist, code);
    } while(character == NULL);
    CHARACTERaddEquipment(character, inv);

}
void CHLISTdelEquipmentToCharacter(ChList chlist, Inventory inv) {
    char code[CODE_LEN+1];
    Character character;

    do {
        printf("Inserisci il codice del personaggio: ");
        scanf("%s", code);
        character = CHLISTsearchByCode(chlist, code);
    } while(character == NULL);
    CHARACTERdelEquipment(character, inv);
}