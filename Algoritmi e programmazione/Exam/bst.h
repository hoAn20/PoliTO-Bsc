#include <stdio.h>
#include <stdlib.h>

#include "piatto.h"
#include "menu.h"

#ifndef BST_H
#define BST_H

typedef struct bst *BST;

BST BSTinit();
void BSTfree(BST bst);
void BSTinsert(BST bst, Menu menu, int P);  //il parametro P serve per poter usare la funzione MENUcompare
BST BSTmenuGenera(Piatto *piatti, int N, int P);
void BSTmenuPrintInOrder(BST bst, Piatto *piatti, int P);

#endif
