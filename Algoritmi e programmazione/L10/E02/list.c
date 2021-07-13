#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node {
    Diagonal diagonal;
    Link next;
};
struct list {
    Link head;
    int N;
};

Link NODEnew(Diagonal diag, Link next) {
    Link new = malloc(sizeof(Node));

    new->diagonal = diag;
    new->next = next;

    return new;
}
void NODEfree(Link del) {
    free(del);
}

List LISTnew() {
    List list = malloc(sizeof(*list));

    list->head = NULL;
    list->N = 0;

    return list;
}
void LISTinsertInHead(List list, Diagonal diag) {
    list->head = NODEnew(diag, list->head);
    list->N++;
}
void LISTfree(List list) {
    Link x, t;

    for(x = list->head; x!= NULL; ) {
        t = x;
        x = x->next;
        NODEfree(t);
    }

    free(list);
}
Diagonal *LISTtoArrayAndDelete(List list) {
    Diagonal *diags = malloc(list->N * sizeof(Diagonal));
    int i = 0;

    for (Link x = list->head; x!= NULL; x = x->next) {
        diags[i] = x->diagonal;
        i++;
    }

    LISTfree(list);
    return diags;
}
int DIAGsize(List list) {
    return list->N;
}
void printHEAD(List list, Element *elems) {
    printf("%s\n\n\n\n", elems[list->head->diagonal.indexElement[0]].name);
}