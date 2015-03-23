#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 35
#define MAX(a,b) ((a)>(b)?(a):(b))

#ifdef DEBUG
#define dprint(...) printf(__VA_ARGS__)
#else
#define dprint(...)
#endif

typedef struct graph_s graph_t;
struct graph_s {
    int edges[MAX_N+1][MAX_N];
    int degree[MAX_N+1];
    int nvertices;
    int nedges;
    int dominates[MAX_N+1][MAX_N+1];
};

graph_t G;

int compare_degree(const void *p1, const void *p2)
{
    return G.degree[*(int *)p2] - G.degree[*(int *)p1];
}

int compare_node(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}


int components[MAX_N][MAX_N];  //components[g][i] = G의 g번째 components의 i번째 노드
int component_cnt;             //그래프 G 내의 컴포넌트 갯수
int component_node_cnt[MAX_N]; //component_node_cnt[g] = G의 g번째 component의 노드 갯수

void add_edge(graph_t *graph, int v1, int v2)
{
    int i;

    for (i = 0; i < graph->degree[v1]; i++) {
        if (graph->edges[v1][i] == v2)
            return;
    }

    graph->edges[v1][graph->degree[v1]++] = v2;
}

void dfs(int n, int visited[])
{
    int v, i;

    for (i = 0; i < G.degree[n]; i++) {
        v = G.edges[n][i];
        if (!visited[v]) {
            visited[v] = 1;
            components[component_cnt][component_node_cnt[component_cnt]++] = v;
            dfs(v, visited);
        }
    }
}

int contains(int a[], int na, int b[], int nb)
{
    //true iff a[] contains b[]
    //assume that a[], b[] are sorted

    int i, j;

    if (nb > na) 
        return 0;

    if (nb == 0 || na == 0) 
        return 0;

    for (i = 0, j = 0; i < nb; i++) {
        while (a[j] != b[i] && j < na) j++;
        if (j == na)
            return 0;
    }

    return 1;
}


void find_dominates()
{
    int v, w, i;

    for (v = 1; v <= G.nvertices; v++) {
        for (w = 1; w <= G.nvertices; w++) {
            if (w == v ||
                contains(G.edges[v], G.degree[v], G.edges[w], G.degree[w])) {
                G.dominates[v][w] = 1;
                if (w != v)
                    dprint("N(%d) > N(%d)\n", v, w);
            }
        }
    }

    dprint("dominates\n");
    for (v = 1; v <= G.nvertices; v++) {
        dprint("%d: ", v);
        for (w = 1; w <= G.nvertices; w++) {
            if (G.dominates[v][w])
                dprint("%d ", w);
        }
        dprint("\n");
    }
}

void find_components()
{
    int v, i, j;
    int visited[MAX_N+1];
    memset(visited, 0, sizeof(visited));
    memset(component_node_cnt, 0, sizeof(component_node_cnt));
    memset(components, 0, sizeof(components));

    component_cnt = 0;

    for (v = 1; v <= G.nvertices; v++) {
        if (!visited[v]) {
            visited[v] = 1;
            components[component_cnt][component_node_cnt[component_cnt]++] = v;
            dfs(v, visited);

            //sort by node number
            qsort(components[component_cnt], component_node_cnt[component_cnt], 
                  sizeof(int), compare_node);

            component_cnt++;
        }
    }

    dprint("find %d components\n", component_cnt);
    for (i = 0; i < component_cnt; i++) {
        dprint("graph %d: ", i);
        for (j = 0; j < component_node_cnt[i]; j++)
            dprint("%d ", components[i][j]);
        dprint("\n");
    }
}

int uniq(int a[], int n)
{
    int i, *pi;

    for (i = 1, pi = a; i < n; i++) {
        if (*pi != a[i]) {
            *(++pi) = a[i];
        }
    }

    return pi - a + 1;
}

int find_candidates(int a[], int k, int c[], int g)
{
    int r = 0;
    int i, j, v, w;
    int t[MAX_N+1];

    if (k == 0) {
        memset(t, 0, sizeof(t));
        dprint("component_node_cnt[%d] = %d\n", g, component_node_cnt[g]);
        for (i = 0; i < component_node_cnt[g]; i++) {
            v = components[g][i];

            if (t[v]) continue;

            t[v] = 1;
            c[r++] = v;

            for (j = 0; j < component_node_cnt[g]; j++) {
                w = components[g][j];
                if (G.dominates[v][w])
                    t[w] = 1;
            }
        }

        dprint("a = []\n");
        dprint("c[%d]: ", r);
        for (i = 0; i < r; i++) {
            dprint("%d ", c[i]);
        }
        dprint("\n");

        return r;
    }

    //a[]의 모든 dominating set 바깥에 있는 노드를 뽑아오면 된다.
    for (i = 0; i < component_node_cnt[g]; i++) {
        v = components[g][i];

        if (a[k-1] >= v)
            continue;

        for (j = 0; j <= k-1; j++) {
            if (G.dominates[a[j]][v])
                break;
        }

        if (j == k) {
            c[r++] = v;
        }
    }

    dprint("a[] = ");
    for (i = 0; i < k; i++) {
        dprint("%d ", a[i]);
    }
    dprint("\n");

    dprint("c[%d]: ", r);
    for (i = 0; i < r; i++) {
        dprint("%d ", c[i]);
    }
    dprint("\n");

    return r;
}


int backtrack(int a[], int k, int n, int g)
{
    int i, j, m, v, cover_len;
    int n_c;
    int c[MAX_N];

    if (k == n) {
        //a[]의 모든 domination set들이 components[g]를 cover하는지
        memset(c, 0, sizeof(c));
        for (i = 0; i < component_node_cnt[g]; i++) {
            v = components[g][i];
            for (j = 0; j < n; j++) {
                if (a[j] == v)
                    break;

                for (m = 0; m < G.degree[a[j]]; m++) {
                    if (v == G.edges[a[j]][m])
                        break;
                }
            }

            if (j == n)
                return 0;
        }

        return 1;
    }

    n_c = find_candidates(a, k, c, g);
    /* if (n_c < n - k) */
        /* return 0; */

    for (i = 0; i < n_c; i++) {
        a[k] = c[i];
        dprint("a[%d] = %d\n", k, a[k]);
        if (backtrack(a, k+1, n, g))
            return 1;
    }

    return 0;
}

void print_graph()
{
    int v, i;

    for (v = 1; v <= G.nvertices; v++) {
        dprint("%d: ", v);
        for (i = 0; i < G.degree[v]; i++) {
            dprint("%d ", G.edges[v][i]);
        }
        dprint("\n");
    }
}

int process()
{
    int v, i;
    int g;
    int max_d, d_num, d;
    int c = 0;
    int a[MAX_N+1];

    //sort edges
    for (v = 1; v <= G.nvertices; v++) {
        qsort(G.edges[v], G.degree[v], sizeof(int), compare_node);
    }

    //print graph
    print_graph();


    //find connected components
    find_components();

    //get domiation info
    find_dominates();

    for (g = 0; g < component_cnt; g++) {
        // get minimum bound of domination number
        // see : http://en.wikipedia.org/wiki/Dominating_set
        max_d = 0;
        for (i = 0; i < component_node_cnt[g]; i++) {
            v = components[g][i];
            max_d = MAX(max_d, G.degree[v]);
        }

        d_num = (component_node_cnt[g]+max_d)/(1+max_d);

        for (d = d_num; d <= component_node_cnt[g]; d++) {
            dprint("d=%d\n", d);
            if (backtrack(a, 0, d, g))
                break;
        }

        c += d;
    }

    return c;
}

int main()
{
    int i;
    int v1, v2;

    while (1) {
        memset(&G, 0, sizeof(G));
        scanf("%d%d", &G.nvertices, &G.nedges);

        if (G.nvertices == 0 && G.nedges == 0)
            break;

        for (i = 1; i <= G.nvertices; i++) {
            add_edge(&G, i, i);
        }


        for (i = 0; i < G.nedges; i++) {
            scanf("%d%d", &v1, &v2);
            add_edge(&G, v1, v2);
            add_edge(&G, v2, v1);
        }

        printf("%d\n", process());
    }

    return 0;
}

