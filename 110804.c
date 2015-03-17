#include <stdio.h>
#include <string.h>

#define MAX_N 35
#define MAX_DEGREE 35
#define MAX(a,b) ((a)>(b)?(a):(b))

int N, M;

#ifdef DEBUG
#define dprint(...) printf(__VA_ARGS__)
#else
#define dprint(...)
#endif

typedef struct graph_s graph_t;
struct graph_s {
    int edges[MAX_N+1][MAX_DEGREE];
    int degree[MAX_N+1];
    int nvertices;
    int nedges;
};

graph_t graph;

int find_candidates(int a[], int k, int c[])
{
    int i, j;
    int r = 0;
    int valid;

    dprint("candidates: ");

    if (k == 0) {
        for (i = 1; i <= N; i++) {
            c[r++] = i;
            dprint("%d ", i);
        }
        dprint("\n");

        return r;
    }

    // a[k]에 들어갈 후보군 결정 
    // return값은 candidates의 갯수

    for (i = 1; i <= N; i++) {
        valid = 1;

        // v[i]에 인접한 노드에 이미 service center가 있다면 패스
        for (j = 0; j <= k-1; j++) {
            if (i == a[j] || graph[a[j]][i]) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            c[r++] = i;
            dprint("%d ", i);
        }
    }

    dprint("\n");

    return r;
}

int backtrack(int a[], int k, int n)
{
    int c[MAX_N];
    int n_c;
    int i, j;
    int valid;

    if (k == n) {
        // 모든 노드에서 service 센터에 접근 가능한지 확인
        for (i = 1; i <= N; i++) {
            valid = 0;
            for (j = 0; j < n; j++) {
                if (a[j] == i || graph[a[j]][i]) {
                    valid = 1;
                    break;
                }
            }

            if (!valid)
                return 0;
        }

        return 1;
    }

    dprint("k = %d, n = %d\n", k, n);
    n_c = find_candidates(a, k, c);

    for (i = 0; i < n_c; i++) {
        a[k] = c[i];
        dprint("a[%d] = %d\n", k, c[i]);
        if (backtrack(a, k+1, n))
            return 1;
    }

    return 0;
}

int process()
{
    int max_d = 0;
    int degree;
    int i, j;
    int d_num;
    int a[MAX_N];

    // get minimum bound of domination number
    // see : http://en.wikipedia.org/wiki/Dominating_set
    for (i = 1; i <= N; i++) {
        max_d = MAX(max_d, graph.degree[i]);
    }

    d_num = N/(1+max_d);

    for (i = d_num; i <= N ; i++) {
        if (backtrack(a, 0, i))
            return i;
    }

    return N;
}

int main()
{
    int i, v1, v2;

    while (1) {
        scanf("%d%d", &N, &M);

        if (N == 0 && M == 0)
            break;

        memset(&graph, 0, sizeof(graph));

        graph.nvertices = M;
        for (i = 0; i < M; i++) {
            scanf("%d%d", &v1, &v2);
            graph.edges[v1][graph.degree[v1]++] = v2;
            graph.edges[v2][graph.degree[v2]++] = v1;
            graph.nedges++;
        }

        printf("%d\n", process());
    }

    return 0;
}


