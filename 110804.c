#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 35

#ifdef DEBUG
#define dprint(...) printf(__VA_ARGS__)
#else
#define dprint(...)
#endif

#define CEIL_DIV(x, y) (((x)+(y)-1)/(y))

int G[MAX_N+1][MAX_N+1];
int d[MAX_N+1];
int ns[MAX_N+1][MAX_N+1] = {0,};
int n_cnt[MAX_N+1] = {0,};
int md[MAX_N+1] = {0,};
int N, M;
int g_cnt = 0;

int input()
{
    int i;
    int v1, v2;

    scanf("%d %d", &N, &M);
    if (N == 0 && M == 0)
        return 0;

    memset(G, 0, sizeof G);
    memset(d, 0, sizeof d);

    for (i = 0; i < M; i++) {
        scanf("%d %d", &v1, &v2);
        G[v1][v2] = 1;
        G[v2][v1] = 1;
        d[v1]++;
        d[v2]++;
    }

    return 1;
}

void dfs(int n, int visited[])
{
    int v, i;
    for (v = 1; v <= N; v++) {
        if (G[v][n] && !visited[v]) {
            visited[v] = 1;
            ns[g_cnt][n_cnt[g_cnt]++] = v;
            dfs(v, visited);
        }
    }
}

int compare_degree(const void *p1, const void *p2)
{
    return d[*(int *)p2] - d[*(int *)p1];
}

void find_components()
{
    int visited[MAX_N+1];
    int v, i, j, g;

    memset(n_cnt, 0, sizeof(n_cnt));
    memset(ns, 0, sizeof(ns));
    memset(visited, 0, sizeof(visited));
    memset(md, 0, sizeof(md));
    g_cnt = 0;

    for (v = 1; v <= N; v++) {
        if (!visited[v]) {
            /* get connected component */
            visited[v] = 1;
            ns[g_cnt][n_cnt[g_cnt]++] = v;
            dfs(v, visited);

            /* get maximum degree of current component */
            for (i = 0; i < n_cnt[g_cnt]; i++) {
                if (d[ns[g_cnt][i]] > md[g_cnt])
                    md[g_cnt] = d[ns[g_cnt][i]];
            }

            qsort(ns[g_cnt], n_cnt[g_cnt], sizeof(int), compare_degree);

            g_cnt++;
        }
    }

    /* debug print .. */
    dprint("find %d components\n", g_cnt);
    for (g = 0; g < g_cnt; g++) {
        dprint("   ");
        for (i = 0; i < n_cnt[g]; i++)
            dprint("%02d ", ns[g][i]);
        dprint("\n");

        for (i = 0; i < n_cnt[g]; i++) {
            dprint("%02d ", ns[g][i]);
            for (j = 0; j < n_cnt[g]; j++) {
                dprint("%c  ", G[ns[g][i]][ns[g][j]] ? '*': ' ');
            }
            dprint("\n");
        }
    }
}

int find_candidates(int a[], int k, int c[], int g)
{
    int v, i, j, cnt;

    if (k == 0) {
        memcpy(c, ns[g], n_cnt[g]*sizeof(int));
        return n_cnt[g];
    }

    for (j = 0; j < n_cnt[g]; j++) {
        if (a[k-1] == ns[g][j]) {
            break;
        }
    }

    for (i = j + 1, cnt = 0; i < n_cnt[g]; i++) {
        v = ns[g][i];
        /*for (j = 0; j < k; j++) {*/
            /*if (G[a[j]][v] || v == a[j])*/
                /*break;*/
        /*}*/

        /*if (j == k)*/
            c[cnt++] = v;
    }

    return cnt;
}

int backtrack(int a[], int k, int n, int g)
{
    int n_c, i, j;
    int c[MAX_N+1];
    int visited[MAX_N+1];

    if (n == k) {
        memset(visited, 0, sizeof(visited));
        for (i = 0; i < n; i++) {
            for (j = 0; j < n_cnt[g]; j++) {
                if (G[a[i]][ns[g][j]] || a[i] == ns[g][j]) {
                    visited[ns[g][j]] = 1;
                }
            }
        }

        dprint("a = ");
        for (i = 0; i < n; i++)
            dprint("%d ", a[i]);

        for (i = 0; i < n_cnt[g]; i++)
            if (visited[ns[g][i]] == 0) {
                dprint("==> not cover %d\n", ns[g][i]);
                return 0;
            }
        dprint("\n");
        return 1;
    }

    n_c = find_candidates(a, k, c, g);

    /*dprint("n_c=%d, n=%d, k=%d\n", n_c, n, k);*/
    if (n_c < n - k) {
        return 0;
    }

    for (i = 0; i < n_c; i++) {
        a[k] = c[i];
        if (backtrack(a, k+1, n, g))
            return 1;
    }

    return 0;
}

int mds(int i) /* i = component index */
{
    int dn;
    int a[MAX_N+1];

    /* |MDS| has lower and upper bound
     * see: http://en.wikipedia.org/wiki/Dominating_set
     */
    dprint("%d <= dn <= %d\n", CEIL_DIV(n_cnt[i], 1 + md[i]), n_cnt[i]/2);
    if (CEIL_DIV(n_cnt[i], 1 + md[i]) == n_cnt[i]/2)
        return CEIL_DIV(n_cnt[i], 1 + md[i]);

    for (dn = CEIL_DIV(n_cnt[i], 1 + md[i]); dn <= n_cnt[i]/2; dn++) {
        /* try to construct dominating set with `dn` vertices */
        dprint("try mds = %d\n", dn);
        if (backtrack(a, 0, dn, i))
            return dn;
    }

    return dn;
}

int process()
{
    int sum = 0;
    int i, j;

    /* debug print */
    dprint("   ");
    for (i = 1; i <= N; i++)
        dprint("%02d ", i);
    dprint("\n");

    for (i = 1; i <= N; i++) {
        dprint("%02d ", i);
        for (j = 1; j <= N; j++) {
            dprint("%c  ", G[i][j] ? '*': ' ');
        }
        dprint("\n");
    }

    find_components();

    for (i = 0; i < g_cnt; i++) {
        dprint("== component %d ==\n", i);
        sum += mds(i);
    }

    return sum;
}

int main()
{
    while (input())
        printf("%d\n", process());

    return 0;
}

