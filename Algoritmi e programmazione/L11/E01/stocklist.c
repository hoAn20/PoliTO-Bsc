#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stocklist.h"

typedef struct node Node, *Link;
struct node {
    Stock stock;
    Link next;
};
struct slist {
    Link head;
    int size;
};

Link LISTnodeNew(Stock stock, Link next);

Link LISTnodeNew(Stock stock, Link next) {
    Link x = malloc(sizeof(*x));
    x->stock = STOCKnew();
    x->stock = stock;
    x->next = next;

    return x;
}

StockLIST SLISTinit() {
    StockLIST stockList = malloc(sizeof(*stockList));
    stockList->head = LISTnodeNew(STOCKsetNull(), NULL);
    stockList->size = 0;

    return stockList;
}
void SLISTfree(StockLIST sList) {
    STOCKfree(sList->head->stock);
    free(sList);
}
void SLISTinsertInOrder(StockLIST sList, Stock stock) {
    Link x, t;
    if (STOCKcompare(sList->head->stock, STOCKsetNull()) == 0 || STOCKcompare(sList->head->stock, stock) > 0)
        sList->head = LISTnodeNew(stock, sList->head);
    else {
        for (x = sList->head; STOCKcompare(x->next->stock, STOCKsetNull())!= 0 && STOCKcompare(x->next->stock, stock) < 0; x = x->next);
        t = LISTnodeNew(stock, x->next);
        x->next = t;
    }

    sList->size++;
}
Stock SLISTsearch(StockLIST sList, Stock stock) {
    Link x;
    if (sList->head == NULL)
        return STOCKsetNull();
    for (x = sList->head; STOCKcompare(x->stock, STOCKsetNull())!= 0 && STOCKcompare(x->stock, stock)!= 0; x = x->next);
    if (x->next == NULL)
        return STOCKsetNull();
    else
        return x->stock;
}
void SLISTprint(StockLIST sList) {
    Link x;
    for (x = sList->head; x->next!= NULL; x=x->next)
        STOCKprint(x->stock);
}
void SLISTdailyStockUpdate(StockLIST sList, Stock stock, FILE *f) {
    Link x;
    for (x = sList->head; x!=NULL && STOCKcompare(x->stock, stock)!= 0; x = x->next);
    STOCKaddTransaction(stock, f);
}
