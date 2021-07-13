#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgList.h"

struct node {
    pg_t character;
    LINK next;
};
struct list {
    LINK head;
    LINK tail;
    int nPG;
};
LIST NEWlist() {
    LIST list;

    list = malloc(sizeof(*list));
    list->head = NULL;
    list->nPG = 0;

    return list;
}
void DELlist(LIST l) {
    free(l);
}
LINK LISTnewNode(pg_t newPg, LINK next) {
    LINK newNode = malloc(sizeof(*newNode));

    newNode->character = newPg;
    newNode->next = next;

    return newNode;
}
void LISTinsertInTail(LIST l, pg_t newPg) {
    LINK tmp;

    if (l->head == NULL) {
        l->head = LISTnewNode(newPg, NULL);
        l->tail = l->head;
    } else {
        tmp = l->tail;
        tmp->next = LISTnewNode(newPg, NULL);
        l->tail = tmp->next;
    }
    l->nPG++;
}
void LISTdelByCode(LIST l, char *code) {
    LINK x, tmp;

    if (CHECKexistence(l, code)) {
        if (strcmp(l->head->character.code, code) == 0) {
            tmp = l->head;
            l->head = tmp->next;
            free(tmp);
        } else {
            for (x = l->head; strcmp(x->next->character.code, code)!= 0; x = x->next);
            tmp = x->next;
            x->next = tmp->next;
            free(tmp);
        }
    } else
        printf("L'elemento che si cerca di eliminare non esiste.\n\n");
}
int CHECKexistence(LIST l, char *code) {
    LINK x;

    for (x = l->head; x!= NULL; x = x->next) {
        if (strcmp(x->character.code, code) == 0)
            return 1;
    }

    return 0;
}
LINK SEARCHbyCode(LIST l, char *code) {
    LINK x;

    for (x = l->head; x!= NULL; x = x->next) {
        if (strcmp(x->character.code, code) == 0)
            return x;
    }

    return NULL;
}
void LISTprint(LIST l) {
    LINK x;

    for (x = l->head; x!= NULL; x = x->next) {
        PRINTpg(x->character);
    }
}