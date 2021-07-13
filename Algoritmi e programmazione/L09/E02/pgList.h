#ifndef PGLIST_H
#define PGLIST_H

#include "Pg.h"

typedef struct list *LIST;
typedef struct node NODE, *LINK;

LIST NEWlist();
void DELlist(LIST l);
LINK LISTnewNode(pg_t newPg, LINK NEXT);
void LISTinsertInTail(LIST l, pg_t newPg);
void LISTdelByCode(LIST l, char *code);
int CHECKexistence(LIST l, char *code);
LINK SEARCHbyCode(LIST l, char *code);
void LISTprint(LIST l);

#endif