/*Determinati daca exista sau nu drum direct intre doua restaurante dintr-o retea de tip graf*/

#include <stdlib.h>
#include <stdio.h>

    typedef struct Node
{
    int data;
    struct Node *next;
}NODE;

/// pentru simplitate, folosim int uri pt a numi restaurantel/locatiile
/// ex: 1 - restaurantul 1 si tot asa

typedef struct g
{
    int v;
    int *vis;
    struct Node **alst;
} GPH;

typedef struct s
{
    int t;
    int scap;
    int *arr;
} STK;

NODE *create_node(int v)
{
    NODE *nn = malloc(sizeof(NODE));
    nn->data = v;
    nn->next = NULL;
    return nn;
}

void add_edge(GPH *g, int src, int dest)
{
    NODE *nn = create_node(dest);
    nn->next = g->alst[src];
    g->alst[src] = nn;
    nn = create_node(src);
    nn->next = g->alst[dest];
    g->alst[dest] = nn;
}

GPH *create_g(int v)
{
    int i;
    GPH *g = malloc(sizeof(GPH));
    g->v = v;
    g->alst = malloc(sizeof(NODE *) * v + 1 );
    g->vis = malloc(sizeof(int) * v + 1 );

    for (int i = 1; i <= v; i++)
    {
        g->alst[i] = NULL;
        g->vis[i] = 0;
    } 
    return g;
}

STK *create_s(int scap)
{
    STK *s = malloc(sizeof(STK)*1000);
    s->arr = malloc(1000 * sizeof(int));
    s->t = 0;
    s->scap = scap;

    return s;
}

void push(int pshd, STK *s)
{
    s->t = s->t + 1;
    s->arr[s->t] = pshd;
}

void DFS(GPH *g, STK *s, int v_nr)
{
    NODE *adj_list = g->alst[v_nr];
    NODE *aux = adj_list;
    int con_ver;
    g->vis[v_nr] = 1;
    push(v_nr, s);
    while (aux != NULL)
    {
        con_ver = aux->data;

        if (g->vis[con_ver] == 0 )
        {
            DFS(g, s, con_ver);

        }

        aux = aux->next;
    }
}

void insert_edges(GPH *g, int edg_nr, int nrv)
{
    int src, dest, i;

    printf("adauga %d munchii (de la 1 la %d)\n", edg_nr, nrv);

    for (i = 1; i <= edg_nr; i++)
    {
        scanf("%d%d", &src, &dest);
        add_edge(g, src, dest);
    }
}

void wipe(GPH *g, int nrv)
{
    for (int i = 0; i <= nrv; i++)
    {
        g->vis[i] = 0;
    }
} 


int *Canbe(GPH *g, int nrv, STK **s1, STK **s2) // 0 sau 1 daca poate fi sau nu ajuns
{
    int *canbe = calloc(nrv * nrv, sizeof(int)); 
    for (int i = 1; i < nrv; i++) // aici i tine loc de numar adica de restaurant
    {
        for (int j = i + 1; j <= nrv; j++) // si j tine loc de numar de restaurant
        {
            DFS(g, *s1, i);
            wipe(g, nrv);
            DFS(g, *s2, j);
            wipe(g, nrv);

            int ans = 0;
            
            for (int l = 1; l <= (*s1)->t && !ans; l++) //parcurgem stack-ul 1
            {
                for (int q = 1; q <= (*s2)->t && !ans; q++) // parcurgem stack-ul 2
                {
                    if (((*s1)->arr[l] == j) && ((*s2)->arr[q] == i))
                    {
                        *(canbe + j + (i - 1) * nrv -1) = *(canbe + i + (j - 1) * nrv -1) = 1;
                        ans = 1;
                    }
                }

             }

            free(*s1); //golim stack-ul 1
            free(*s2); //golim stack-ul 2

            *s1 = create_s(2 * nrv); //realocam spatiu pentru stack-ul 1
            *s2 = create_s(2 * nrv); //realocam spatiu pentru stack-ul 2

             
        }
    }

    return canbe;

}

int main()
{

    int nrv, edg_nr, src, dest, i, vortex_1, virtex_2, ans,*smt;

    printf("cate noduri are graful?");
    scanf("%d", &nrv);

    printf("cate muchii are graful?");
    scanf("%d", &edg_nr);

    GPH *g = create_g(nrv);
    

    STK *s1 = create_s(2 * nrv);
    STK *s2 = create_s(2 * nrv);

    insert_edges(g, edg_nr, nrv );

    int *m;
    m = Canbe(g, nrv, &s1, &s2);
    
    for(int i = 0;i < nrv * nrv; i++)
    {
        if(*(m + i))
        {
            printf("Exista drum intre %d si %d.\n",(i / nrv + 1),( i % nrv + 1));
        }

        else

        if((i / nrv + 1)!=(i % nrv + 1))
        {
            printf("Nu exista drum intre %d si %d.\n",( i / nrv + 1),( i % nrv + 1));

        }
    }
}