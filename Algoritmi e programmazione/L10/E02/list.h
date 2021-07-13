#include "element.h"
#include "diagonal.h"

#ifndef LIST_H
#define LIST_H

typedef struct node Node, *Link;
typedef struct list *List;

List LISTnew();
void LISTinsertInHead(List list, Diagonal diag);
void LISTfree(List list);
Diagonal *LISTtoArrayAndDelete(List list);
int DIAGsize(List list);
void printHEAD(List list, Element *elems);

#endif
