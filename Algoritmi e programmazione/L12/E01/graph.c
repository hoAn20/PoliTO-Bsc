#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "graph.h"

typedef struct node* link;

struct node{
    int v;
    int wt;
    link next;
};

typedef struct{
    int v;
    int w;
    int wt;
} Edge;

struct graph{
    int V;
    int E;
    link* lAdj;
    link z;
    SymTab tab;
    Edge* edges;
};

Graph GRAPHinit();
link NEW(int a, int wt, link next);
Edge EDGEcreate(int v, int w, int wt);
void PrintEdges(Graph G, int *edgesConsidered);
int GRAPHdfs(Graph G, int id, int *edgesConsidered);
int dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st, int *edgesConsidered);
void TSdfsR(Graph G, int v, int *ts, int *pre, int *time, int *edgesConsidered);
int ConsiderableEdges(int v, int w, Graph G, int *edgesConsidered);
void DAGrts(Graph G, int* edgesConsidered);
void PrintMaxDist(Graph G, int *edgesConsidered, int *ts);
int PowerSet(Graph G, int m, int pos, int *sol, int* solMax, int* maxWt);

Graph GRAPHinit(){
    Graph g = malloc(sizeof(struct graph));

    g->V = 0;
    g->E = 0;
    g->z = NEW(-1,0, NULL);
    g->tab.size = 0;
    g->tab.a = (Item*)malloc(sizeof(Item) * g->V);

    return g;
}
link NEW(int a,int wt, link next) {
    link x = malloc(sizeof *x);

    x->v = a;
    x->wt = wt;
    x->next = next;

    return x;
}
Edge EDGEcreate(int v, int w, int wt) {
    Edge e;

    e.v = v;
    e.w = w;
    e.wt = wt;

    return e;
}
void PrintEdges( Graph G, int* edgesConsidered){
    int i;

    for (i = 0; i < G->E; i++) {
        if (edgesConsidered[i] == 1)
            printf("%s %s %d\n", STgetNameFromIndex(G->tab,G->edges[i].v), STgetNameFromIndex(G->tab,G->edges[i].w),G->edges[i].wt);
    }
    printf("\n\n");
}
int GRAPHdfs(Graph G, int id, int* edgesConsidered) {
    int v, time = 0, *pre, *post, *st, check = 0;

    pre = malloc(G->V * sizeof(int));
    post = malloc(G->V * sizeof(int));
    st = malloc(G->V * sizeof(int));
    for (v = 0; v < G->V; v++) {
        pre[v] = -1;
        post[v] = -1;
        st[v] = -1;
    }

    if( dfsR(G, EDGEcreate(id, id, 0), &time, pre, post, st, edgesConsidered)){
        check = 1;

    }
    if(!check)
        return 0;
    for (v = 0; v < G->V; v++) {
        if (pre[v] == -1) {
            if(dfsR(G, EDGEcreate(v, v, 0), &time, pre, post, st, edgesConsidered))
                check = 1;
        }
    }

    return check;
}
int dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st, int* edgesConsidered){
    link t;
    int i, v, w;

    for (i = 0; i < G->E; ++i) {
        if(e.v == G->edges[i].v && e.w == G->edges[i].w && edgesConsidered[i] == 0)
            return -1;
    }

    w = e.w;
    st[e.w] = e.v;
    pre[w] = (*time)++;
    for (t = G->lAdj[w]; t != G->z; t = t->next) {
        if (pre[t->v] == -1) {
            if (!dfsR(G, EDGEcreate(w, t->v, t->wt), time, pre, post, st, edgesConsidered))
                return 0;
        }
        else if (ConsiderableEdges(w, t->v, G, edgesConsidered)){
            v = t->v;
            if (post[v] == -1)    //caso di arco back
                return 0;
        }
    }
    post[w] = (*time)++;

    return 1;
}
void TSdfsR(Graph G, int v, int *ts, int *pre, int *time, int* edgesConsidered) {
    link t;

    pre[v] = 0;
    for (t = G->lAdj[v]; t != G->z; t = t->next) {
        if (pre[t->v] == -1 && ConsiderableEdges(v, t->v, G, edgesConsidered))
            TSdfsR(G, t->v, ts, pre, time, edgesConsidered);
    }
    ts[(*time)++] = v;
}
void DAGrts(Graph G, int* edgesConsidered) {
    int v, time = 0, *pre, *ts, i ,tmp;

    pre = malloc(G->V * sizeof(int));
    ts = malloc(G->V * sizeof(int));
    for (v = 0; v < G->V; v++) {
        pre[v] = -1;
        ts[v] = -1;
    }
    for (v = 0; v < G->V; v++)
        if (pre[v] == -1)
            TSdfsR(G, v, ts, pre, &time, edgesConsidered);
    for (i = 0; i < G->V / 2; ++i) {
        tmp = ts[i];
        ts[i] = ts[G->V - 1 - i];
        ts[G->V - 1 - i] = tmp;
    }

    PrintMaxDist(G, edgesConsidered, ts);
}
void PrintMaxDist(Graph G, int *edgesConsidered, int *ts) {
    int *d = malloc(G->V * sizeof(int));
    int i, j;
    link t;

    for (j = 0; j < G->V; ++j) {
        for (i = 0; i < G->V; ++i)
            d[i] = INT_MIN;
        d[ts[j]] = 0;

        for (i = j; i < G->V; ++i) {
            if (d[ts[i]] != INT_MIN) {
                for (t = G->lAdj[ts[i]]; t != G->z; t = t->next) {
                    if (d[t->v] < d[ts[i]] + t->wt && ConsiderableEdges(ts[i], t->v, G, edgesConsidered)) {
                        d[t->v] = d[ts[i]] + t->wt;
                    }
                }
            }
        }

        printf("Start: %d \n", ts[j]);
        for (i = 0; i < G->V; ++i) {
            printf("%s %d ", STgetNameFromIndex(G->tab,i) ,(d[i] == INT_MIN)? -1 : d[i]);
        }
        printf("\n");
    }
}
int ConsiderableEdges(int v, int w, Graph G, int *edgesConsidered) {
    int i;
    for (i = 0; i < G->E; ++i) {
        //Non considero multigrafi
        if (v == G->edges[i].v && w == G->edges[i].w && edgesConsidered[i] == 0)
            return 0;
    }

    return 1;
}
int PowerSet(Graph G,int m,int pos,int *sol,int* solMax,int* maxWt) {
    int i, count, wt = 0, sol1, sol2;

    if (pos >= G->E) {
        count = 0;
        for (i = 0; i < G->E; ++i){
            if (sol[i] == 1)
                count++;
            else
                wt += G->edges[i].wt;
        }
        if(count == m){
            if(GRAPHdfs(G, 0, sol)) {
                printf("Numero archi soluzione: %d\n",m);
                printf("Numero archi rimossi: %d\n", G->E - m);
                printf("Archi rimossi: ");
                for (i = 0; i < G->E; ++i) {
                    if(sol[i] == 0)
                        printf("{%d %d} ", G->edges[i].v, G->edges[i].w);
                }
                printf("\n\n");
                PrintEdges(G,sol);
                if(wt >= *maxWt){
                    *maxWt = wt;
                    for (i = 0; i < G->E; ++i)
                       solMax[i] = sol[i];
                }

                return 1;
            }
        }

        return 0;
    }
    sol[pos] = 0;
    sol1 = PowerSet(G, m, pos + 1, sol, solMax, maxWt);
    sol[pos] = 1;
    sol2 = PowerSet(G, m, pos + 1, sol, solMax, maxWt);

    if(sol1 || sol2)
        return 1;

    return 0;
}

Graph GRAPHreadFromFile(char* fileName){
    Graph g = GRAPHinit();
    FILE* f = fopen(fileName, "r");
    char name1[MAX_STR_LEN];
    char name2[MAX_STR_LEN];
    int wt;
    int ind1, ind2;

    if(f == NULL){
        printf("Errore apertura file %s", fileName);
        exit(-1);
    }
    fscanf(f,"%d", &(g->V));

    g->lAdj = (link*)malloc(sizeof(link) * g->V);
    int i;
    for (i = 0; i < g->V; ++i) {
        g->lAdj[i] = g->z;
    }
    g->tab.a = (Item*)malloc(sizeof(Item) * g->V);
    g->tab.size = g->V;
    for (i = 0; i < g->V; ++i) {
        fscanf(f, "%s", g->tab.a[i].name);
    }
    g->edges = (Edge*)malloc(sizeof(Edge) * g->V * 2);
    i = 0;
    while(fscanf(f, "%s %s %d", name1, name2, &wt) != EOF){
        ind1 = STgetIndexFromName(g->tab, name1);
        ind2 = STgetIndexFromName(g->tab, name2);

        if(ind1 == -1 || ind2 == -1){
            printf("Errore\n");
            exit(-1);
        }

        g->E++;
        g->lAdj[ind1] = NEW(ind2, wt, g->lAdj[ind1]);
        g->edges[i++] = EDGEcreate(ind1,ind2, wt);
    }
    g->edges = realloc(g->edges, g->E * sizeof(Edge));

    return g;
}
void GRAPHfindDag(Graph g){
    int i;
    int* sol= calloc(g->E,sizeof(int));
    int* solMax= calloc(g->E,sizeof(int));
    int maxWt = 0;

    for (i = g->E; i > 0; --i) {
        //la funzione powerset va a cercare gli insiemi a cardinalitá massima che
        //rispettano la richiesta di rendere il grafo un dag
        if(PowerSet(g,i, 0, sol,solMax,&maxWt))
            break;
    }

    printf("La soluzione a cardinalita' massima: \n");
    PrintEdges(g,solMax);

    //dag ottenuto dall' elimizaione dell'insieme di archi con peso massio
    DAGrts(g, solMax);

}
