#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stockbst.h"

typedef struct bstnode *Link;
struct bstnode {
    DailyStock dStock;
    Link left;
    Link right;
    Link parent;
    int nSon;
};
struct sbst {
    Link root;
    Link z;     //nodo sentinella
};

Link TREEnewNode(DailyStock dStock, Link left, Link right, Link parent, int nSon);
void TREEfree(Link subRoot, Link z);
DailyStock TREEsearchRecursive(Link subRoot, Date date, Link z);
DailyStock TREEminRecursive(Link subRoot, Link z);
DailyStock TREEmaxRecursive(Link subRoot, Link z);
Link TREEinsertRecursive(Link subRoot, DailyStock dStock, Link z);
Link TREErotationRight(Link subRoot);
Link TREErotationLeft(Link subRoot);
Link TREEpartitionRecursive(Link subRoot, int r);
Link TREEbalanceRecursive(Link subRoot, Link z);
void TREEvisitRecursive(Link subRoot, Link x, Date date1, Date date2, float *min, float *max);

Link TREEnewNode(DailyStock dStock, Link left, Link right, Link parent, int nSon) {
    Link x = malloc(sizeof(*x));
    x->dStock = dStock;
    x->left = left;
    x->right = right;
    x->parent = parent;
    x->nSon = nSon;

    return x;
}
void TREEfree(Link subRoot, Link z) {
    if (subRoot == z)
        return;
    TREEfree(subRoot->left, z);
    TREEfree(subRoot->right, z);
    free(subRoot);
}
int TREEcountRecursive(Link subRoot, Link z) {
    if (subRoot == z)
        return 0;
    return TREEcountRecursive(subRoot->left, z) + TREEcountRecursive(subRoot->right, z) + 1;
}
DailyStock TREEsearchRecursive(Link subRoot, Date date, Link z) {
    int cmp;

    if (subRoot == z)
        return DSTOCKsetNull();
    cmp = DATEcompare(subRoot->dStock.date, date);
    if (cmp == 0)
        return subRoot->dStock;
    else if (cmp < 0)
        return TREEsearchRecursive(subRoot->left, date, z);
    else
        return TREEsearchRecursive(subRoot->right, date, z);
}
DailyStock TREEminRecursive(Link subRoot, Link z) {
    if (subRoot == z)       //caso albero vuoto
        return DSTOCKsetNull();
    if (subRoot->left == z)
        return subRoot->dStock;
    return TREEminRecursive(subRoot->left, z);
}
DailyStock TREEmaxRecursive(Link subRoot, Link z) {
    if (subRoot == z)       //caso albero vuoto
        return DSTOCKsetNull();
    if (subRoot->right == z)
        return subRoot->dStock;
    return TREEmaxRecursive(subRoot->right, z);
}
Link TREEinsertRecursive(Link subRoot, DailyStock dStock, Link z) {
    int cmp;

    if (subRoot == z)       //caso (sotto)albero vuoto
        return TREEnewNode(dStock, z, z, z, 1);
    cmp = DATEcompare(subRoot->dStock.date, dStock.date);
    if (cmp > 0) {
        subRoot->left = TREEinsertRecursive(subRoot->left, dStock, z);
        subRoot->left->parent = subRoot;
        subRoot->nSon++;
    } else if (cmp < 0) {
        subRoot->right = TREEinsertRecursive(subRoot->right, dStock, z);
        subRoot->right->parent = subRoot;
        subRoot->nSon++;
    } else {
        DSTOCKupdate(&(subRoot->dStock), dStock.value, dStock.nStockSum);
    }
    return subRoot;
}
Link TREErotationRight(Link subRoot) {
    Link x = subRoot->left;
    subRoot->left = x->right;
    x->right->parent = subRoot;
    x->right = subRoot;
    x->parent = subRoot->parent;
    subRoot->parent = x;
    x->nSon = subRoot->nSon;
    subRoot->nSon = 1;
    if (subRoot->right)
        subRoot->nSon+= subRoot->right->nSon;
    if (subRoot->left)
        subRoot->nSon+= subRoot->left->nSon;

    return x;
}
Link TREErotationLeft(Link subRoot) {
    Link x = subRoot->right;
    subRoot->right = x->left;
    x->left->parent = subRoot;
    x->left = subRoot;
    x->parent = subRoot->parent;
    subRoot->parent = x;
    x->nSon = subRoot->nSon;
    subRoot->nSon = 1;
    if (subRoot->right)
        subRoot->nSon+= subRoot->right->nSon;
    if (subRoot->left)
        subRoot->nSon+= subRoot->left->nSon;

    return x;
}
Link TREEpartitionRecursive(Link subRoot, int r) {
    int t = subRoot->left->nSon;
    if (t > r) {
        subRoot->left = TREEpartitionRecursive(subRoot->left, r);
        subRoot = TREErotationRight(subRoot);
    }
    if (t < r) {
        subRoot->right = TREEpartitionRecursive(subRoot->right, r-t-1);
        subRoot = TREErotationLeft(subRoot);
    }

    return subRoot;
}
Link TREEbalanceRecursive(Link subRoot, Link z) {
    int r;
    if (subRoot == z)
        return z;
    r = (subRoot->nSon + 1)/ 2 - 1;
    subRoot = TREEpartitionRecursive(subRoot, r);
    subRoot->left = TREEbalanceRecursive(subRoot->left, z);
    subRoot->right = TREEbalanceRecursive(subRoot->right, z);

    return subRoot;
}
void TREEvisitRecursive(Link subRoot, Link z, Date date1, Date date2, float *min, float *max) {
    if (subRoot == z)
        return;

    if (DATEisBetween(date1, subRoot->dStock.date, date2)) {
        if (subRoot->dStock.value > *max)
            *max = subRoot->dStock.value;
        if (subRoot->dStock.value < *min)
            *min = subRoot->dStock.value;
        TREEvisitRecursive(subRoot->left, z, date1, date2, min, max);
        TREEvisitRecursive(subRoot->right, z, date1, date2, min, max);
    }
    if (DATEcompare(subRoot->dStock.date, date2) > 0)
        TREEvisitRecursive(subRoot->left, z, date1, date2, min, max);
    if (DATEcompare(date1, subRoot->dStock.date) > 0)
        TREEvisitRecursive(subRoot->right, z, date1, date2, min, max);
}

StockBST SBSTinit() {
    StockBST sBst = malloc(sizeof(*sBst));
    sBst->root = (sBst->z = TREEnewNode(DSTOCKsetNull(), NULL, NULL, NULL, 0));

    return sBst;
}
void SBSTfree(StockBST sBst) {
    if (sBst == NULL)
        return;
    TREEfree(sBst->root, sBst->z);
    free(sBst->z);
    free(sBst);
}
DailyStock SBSTsearch(StockBST sBst, Date date) {
    return TREEsearchRecursive(sBst->root, date, sBst->z);
}
void SBSTinsertLeafRecursive(StockBST sBst, DailyStock dStock) {
    sBst->root = TREEinsertRecursive(sBst->root, dStock, sBst->z);
}
DailyStock SBSTmin(StockBST sBst) {
    return TREEminRecursive(sBst->root, sBst->z);
}
DailyStock SBSTmax(StockBST sBst) {
    return TREEmaxRecursive(sBst->root, sBst->z);
}
void SBSTbalance(StockBST sBst, int S) {
    if (abs(sBst->root->left->nSon - sBst->root->right->nSon) >= S)
        sBst->root = TREEbalanceRecursive(sBst->root, sBst->z);
}
void SBSTvisit(StockBST sBst, Date date1, Date date2, float *min, float *max) {
    *min = 1000000;
    *max = 0;
    DATEprint(date1);
    DATEprint(date2);
    SBSTbalance(sBst, 0);
    TREEvisitRecursive(sBst->root, sBst->z, date1, date2, min, max);
}
