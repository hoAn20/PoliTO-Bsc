#include "bst.h"

int cnt = 0;

typedef struct bstnode *Link;
struct bstnode {
    Menu menu;
    Link l;
    Link r;
};
struct bst {
    Link root;
    Link z;
};

Link NEW (Menu m, Link l, Link r);
void TreeFreeRec(Link root, Link z);
void GeneraRec(BST bst, Piatto *piatti, int *qta, int *scelte, int N, int P, int pos, int start);
                        //aggiunti i parametri N e P, che venivano passati dal chiamante e usati
                        //senza però essere dichiarti nel prototipo
void VisitRec(Link root, Link z, Piatto *piatti, int P);

BST BSTinit() {
    Menu m;
    BST bst = malloc(sizeof(struct bst));

    m.scelte = calloc(3, sizeof(int));
    m.costo = 0.0;

    bst->root = bst->z = NEW(m, NULL, NULL);

    free(m.scelte);
    return bst;
}
void BSTfree(BST bst) {
    TreeFreeRec(bst->root, bst->z);

    free(bst->z);
    free(bst);
}
void BSTinsert(BST bst, Menu menu, int P) {
    if (bst->root == bst->z) {
        bst->root = NEW(menu, bst->z, bst->z);
        return;
    }

    Link p, x = bst->root;
    while(x != bst->z) {
        p = x;
        if (MENUcompare(menu, x->menu, P) < 0)
            x = x->l;
        else
            x = x->r;
    }
    x = NEW(menu, bst->z, bst->z);
    if (MENUcompare(x->menu, p->menu, P) < 0)
        p->l = x;
    else
        p->r = x;
}
BST BSTmenuGenera(Piatto *piatti, int N, int P) {
    int *qta = calloc(N, sizeof(int)), *scelte = malloc(P * sizeof(int));
    BST bst = BSTinit();

    GeneraRec(bst, piatti, qta, scelte, N, P, 0, 0);

    printf("\n\n\n%d\n\n\n", cnt);

    free(qta);  //riga aggiunta
    free(scelte);  //riga aggiunta

    return bst;
}
void BSTmenuPrintInOrder(BST bst, Piatto *piatti, int P) {
    VisitRec(bst->root, bst->z, piatti, P);
}

Link NEW (Menu m, Link l, Link r) {
    Link x = malloc(sizeof(*x));

    x->menu = m;
    x->l = l;
    x->r = r;

    return x;
}
void TreeFreeRec(Link root, Link z) {
    if (root == z)
        return;

    TreeFreeRec(root->l, z);
    TreeFreeRec(root->r, z);
    //free(root->menu.scelte);
    free(root);
}
void GeneraRec(BST bst, Piatto *piatti, int *qta, int *scelte, int N, int P, int pos, int start) {
    if (pos >= P) {
        cnt++;
        Menu m;
        m.scelte = malloc(P * sizeof(int));
        m.costo = 0.0;
        for (int i = 0; i < P; i++) {
            m.scelte[i] = scelte[i];
            m.costo += piatti[scelte[i]].costo;
        }

        BSTinsert(bst, m, P);

        return;
    }

    for (int i = start; i < N; i++) {
        if (qta[i] < 2) {
            scelte[pos] = i;
            qta[i]++;
            GeneraRec(bst, piatti, qta, scelte, N, P, pos+1, i);
            qta[i]--;
        }
    }
}
void VisitRec(Link root, Link z, Piatto *piatti, int P) {
    if (root == z)
        return;

    VisitRec(root->l, z, piatti, P);
    MENUprint(root->menu, piatti, P);
    VisitRec(root->r, z, piatti, P);
}
