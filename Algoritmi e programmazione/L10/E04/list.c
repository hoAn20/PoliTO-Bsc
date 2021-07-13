#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node {
    int key;
    Link next;
};
struct list {
    Link head;
    int n;
};

Link NODEnew(int index, Link next) {
    Link new = malloc(sizeof(*new));
    new->key = index;
    new->next = next;

    return new;
}
void NODEfree(Link del) {
    free(del);
}

List LISTnew() {
    List list = malloc(sizeof(*list));
    list->head = NULL;
    list->n = 0;

    return list;
}
void LISTinsertNode(List list, int index) {
    list->head = NODEnew(index, list->head);
}
void LISTfree(List list) {
    Link x, t;
    if (list->head != NULL) {
        for (x = list->head->next, t = list->head; x->next != NULL; x = x->next) {
            NODEfree(t);
            t = x;
        }
        NODEfree(t);
    }
    free(list);
}
int LISTpresenceCheck(List list, int key) {
    for (Link x = list->head; x != NULL; x = x->next) {
        if (x->key == key)
            return 1;
    }

    return 0;
}
