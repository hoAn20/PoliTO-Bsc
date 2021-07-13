#ifndef LIST_H
#define LIST_H

typedef struct node Node, *Link;
typedef struct list *List;

List LISTnew();
void LISTinsertNode(List list, int index);
void LISTfree(List list);
int LISTpresenceCheck(List list, int key);

#endif
