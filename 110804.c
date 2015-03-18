#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 35
#define MAX_DEGREE 35
#define MAX(a,b) ((a)>(b)?(a):(b))

int N, M;

#ifdef DEBUG
#define dprint(...) printf(__VA_ARGS__)
#else
#define dprint(...)
#endif

/*typedef struct graph_s graph_t;*/
/*struct graph_s {*/
    /*int edges[MAX_N+1][MAX_DEGREE];*/
    /*int degree[MAX_N+1];*/
    /*int nvertices;*/
    /*int nedges;*/
/*};*/

int graph[MAX_N+1][MAX_N+1];
int degrees[MAX_N+1];

int subgraph[MAX_N+1][MAX_N+1];
int sub_node_cnt[MAX_N+1];
int subgraph_cnt;

int compare_degree(const void *p1, const void *p2)
{
    return degrees[*(int *)p2] - degrees[*(int *)p1];
}

int compare_node(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}

int find_candidates(int a[], int k, int c[], int g)
{
    int i, j;
    int r = 0;
    int valid;

    if (k == 0) {
        for (i = 0; i < sub_node_cnt[g]; i++) {
            c[r++] = subgraph[g][i];
        }

        /* qsort(c, r, sizeof(int), compare_degree); */

        return r;
    }

    // a[k]에 들어갈 후보군 결정 
    // return값은 candidates의 갯수

    for (i = 0; i < sub_node_cnt[g]; i++) {
        if (a[k-1] >= subgraph[g][i])
            continue;

        valid = 1;

        // v[i]에 인접한 노드에 이미 service center가 있다면 패스
        for (j = 0; j <= k-1; j++) {
            if (graph[a[j]][subgraph[g][i]]) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            c[r++] = subgraph[g][i];
        }
    }

    /* qsort(c, r, sizeof(int), compare_degree); */

    return r;
}

int backtrack(int a[], int k, int n, int g)
{
    int c[MAX_N+1];
    int n_c;
    int i, j;
    int valid;

    if (k == n) {
        // 모든 노드에서 service 센터에 접근 가능한지 확인
        for (i = 0; i < sub_node_cnt[g]; i++) {
            valid = 0;
            for (j = 0; j < n; j++) {
                if (graph[a[j]][subgraph[g][i]]) {
                    valid = 1;
                    break;
                }
            }

            if (!valid)
                return 0;
        }

        return 1;
    }

    n_c = find_candidates(a, k, c, g);

    if (n_c < n - k)
        return 0;

    for (i = 0; i < n_c; i++) {
        a[k] = c[i];
        if (backtrack(a, k+1, n, g))
            return 1;
    }

    return 0;
}

int neighbors(int a, int n[])
{
    int r = 0;
    int i = 0;

    /* dprint("N(%d): ",a); */

    /* n[r++] = a; */
    /* dprint("%d ", a); */
    for (i = 1; i <= N; i++) {
        if (graph[a][i]) {
            n[r++] = i;
            /* dprint("%d ", i); */
        }
    }
    /* dprint("\n"); */

    return r;
}

int contains(int a[], int na, int b[], int nb)
{
    //check if a contains b
    int i = 0, j = 0;

    if (nb == 0 || na == 0)
        return 0;

    if (nb > na) 
        return 0;

    while (i < na && j < nb) {
        if (b[j] > a[i]) {
            i++;
        } else if (b[j] < a[i]) {
            return 0;
        } else {
            i++;
            j++;
        }
    }

    return 1;
}

void swap_node(int n1, int n2)
{
    int i,j,t;

    for (i = 1; i <= N; i++) {
        if (graph[i][n1] && graph[i][n2])
            continue;

        if (graph[i][n1]) {
            graph[i][n2] = 1;
            graph[i][n1] = 0;
        } else if (graph[i][n2]) {
            graph[i][n1] = 1;
            graph[i][n2] = 0;
        }
    }

    //swap line
    for (i = 1; i <= N; i++) {
        t = graph[n1][i];
        graph[n1][i] = graph[n2][i];
        graph[n2][t]= t;
    }
}

void prune_graph()
{
    int n1[MAX_N];
    int n_n1;
    int n2[MAX_N];
    int n_n2;
    int i,j;

    // adjust graph by removing non-dominating nodes
    for (i = N; i >= 2; i--) {
        n_n1 = neighbors(i, n1);
        for (j = i-1; j >= 1; j--) {
            n_n2 = neighbors(j, n2);
            //i의 주변노드가 j의 주변노드를 모두 포함하면 j를 없애도 된다.

            if (contains(n1, n_n1, n2, n_n2)) {
                dprint("N(%d) > N(%d)\n", i, j);
                swap_node(j, N);
                N--;
            }
        }
    }

}

void print_graph()
{
    int i, j;

    dprint("  ");
    for (i = 1; i <= N; i++) {
        dprint("%2d ", i);
    }
    dprint("\n");

    for (i = 1; i <= N; i++) {
        dprint("%2d ", i);
        for (j = 1; j <= N; j++) {
            if (graph[i][j]) {
                dprint("*  ");
            } else {
                dprint("   ");
            }
        }
        dprint("\n");
    }
}

void dfs(int n, int visited[])
{
    int i;

    for (i = 1; i <= N; i++) {
        if (graph[n][i] && !visited[i]) {
            visited[i] = 1;
            subgraph[subgraph_cnt][sub_node_cnt[subgraph_cnt]++] = i;
            dfs(i, visited);
        }
    }
}

void find_components()
{
    int i, j;
    int visited[MAX_N+1];
    memset(visited, 0, sizeof(visited));
    memset(sub_node_cnt, 0, sizeof(sub_node_cnt));
    memset(subgraph, 0, sizeof(subgraph));

    subgraph_cnt = 0;

    for (i = 1; i <= N; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            subgraph[subgraph_cnt][sub_node_cnt[subgraph_cnt]++] = i;
            dfs(i, visited);

            //sort by node number
            qsort(subgraph[subgraph_cnt], sub_node_cnt[subgraph_cnt], 
                  sizeof(int), compare_node);

            subgraph_cnt++;
        }
    }

    dprint("find %d components\n", subgraph_cnt);
    for (i = 0; i < subgraph_cnt; i++) {
        dprint("graph %d: ", i);
        for (j = 0; j < sub_node_cnt[i]; j++)
            dprint("%d ", subgraph[i][j]);
        dprint("\n");
    }
}


int process()
{
    int max_d;
    int degree;
    int i, j, g;
    int d_num;
    int a[MAX_N+1];
    int new_n;
    int *pi, *pj;
    int c = 0;

    /* print_graph(); */
    /* prune_graph(); */
    find_components();
    print_graph();

    for (g = 0; g < subgraph_cnt; g++) {
        // get minimum bound of domination number
        // see : http://en.wikipedia.org/wiki/Dominating_set
        max_d = 0;
        for (i = 0; i < sub_node_cnt[g]; i++) {
            d_num = 0;
            for (j = 0; j < sub_node_cnt[g]; j++)
                d_num += graph[subgraph[g][i]][subgraph[g][j]];
            max_d = MAX(max_d, d_num);
        }

        d_num = (sub_node_cnt[g]+max_d)/(1+max_d);

        for (i = d_num; i <= sub_node_cnt[g]; i++) {
            dprint("i=%d\n", i);
            if (backtrack(a, 0, i, g))
                break;
        }

        c += i;
    }

    return c;
}

int main()
{
    int i, v1, v2;

    while (1) {
        scanf("%d%d", &N, &M);

        if (N == 0 && M == 0)
            break;

        memset(graph, 0, sizeof(graph));
        memset(degrees, 0, sizeof(degrees));

        for (i = 0; i <= N; i++) {
            graph[i][i] = 1;
        }


        for (i = 0; i < M; i++) {
            scanf("%d%d", &v1, &v2);
            graph[v1][v2] = 1;
            graph[v2][v1] = 1;
            degrees[v1]++;
            degrees[v2]++;
        }

        /* print_graph(); */
        /* prune_graph(); */
        /* dprint("\n\n"); */
        /* print_graph(); */

        printf("%d\n", process());
    }

    return 0;
}


