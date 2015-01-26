#include <stdio.h>
#include <string.h>

int num_scenario;
int num_paper;
int num_names;

char names[100][80]; /* 그래프 최대크기 : num_names */
char papers[100][100];
int authors[100];

int graph[100][100]; /* 저자간 관계를 나타내는 그래프 */
int d[100]; /* 다익스트라 알고리즘 돌릴때 쓸 거리 저장 배열*/
int q[100]; /* q[v] >= 0 <=> v in Q */

#define MAX_NUM (2<<20)
 
void input()
{
    int i, j = 0, p, n, num_authors, m;
    char buf[BUFSIZ];
    char *pc;
    char *paper;
    char *name;

    num_names = 0;

    memset(graph, 0, 100*100*sizeof(int));
    memset(names, 0, 100*80);
    memset(papers, 0, 100*100);
    memset(authors, 0, 100*sizeof(int));

    scanf("%d %d", &num_paper, &m);

    fgetc(stdin);
    for (p = 0; p < num_paper; p++) {
        num_authors = 0;
        fgets(buf, BUFSIZ - 1, stdin);
        buf[strlen(buf) - 1] = 0; /*remove newline char*/

        pc = strchr(buf, ':');
        paper = pc + 2;

        name = buf;
        pc = buf;
        while (1) {
            if (*pc == '.' && (*(pc + 1) == ',' || *(pc + 1) == ':')) {
                *(pc + 1) = 0;
                /* name is avail */
                /*printf("name=%s\n", name);*/
                for (i = 0; i < num_names; i++) {
                    if (strcmp(names[i], name) == 0)
                        break;
                }

                if (i == num_names) { /*newly introduced name*/
                    strcpy(names[num_names++], name);
                }
                authors[num_authors++] = i;

                name = pc + 3;
                pc = name;

                if (name == paper) 
                    break;
            } else {
                pc++;
            }
        }

        /* debug output */
        /*printf("paper name: %s\n", paper);
        for (i = 0; i < num_authors; i++) {
            printf("--%s(%d)\n", names[authors[i]], authors[i]);
        }*/

        /* author들끼리 연관지어주기*/
        for (i = 0; i < num_authors - 1; i++) {
            for (j = i + 1; j < num_authors; j++) {
                graph[authors[i]][authors[j]] = 1;
                graph[authors[j]][authors[i]] = 1;
            }
        }
    }

    memset(authors, 0, 100*sizeof(int));

    for (n = 0; n < m; n++) {
        /* erdos number 구할사람들 */
        fgets(buf, BUFSIZ - 1, stdin);
        buf[strlen(buf) - 1] = 0;
        for (i = 0; i < num_names; i++) {
            if (strcmp(names[i], buf) == 0)
                authors[n] = i;
        }
    }
    authors[n] = -1;

    /* debug output of graph */
    /*for (i = 0; i < num_names; i++) {
        for (j = 0; j < num_names; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }*/
}

int q_empty()
{
    int v;
    for (v = 0; v < num_names; v++) {
        if (q[v] >= 0) return 0;
    }

    return 1;
}

int q_extract_min()
{
    int min = MAX_NUM;
    int v, min_u = -1;

    for (v = 0; v < num_names; v++) {
        if (q[v] <= min && q[v] >= 0) {
            min = q[v];
            min_u = v;
        }
    }

    q[min_u] = -1; /* remove from Q */
    return min_u;
}

void process()
{
    int v, s, u;
    memset(d, 0, 100*sizeof(int));

    for (v = 0; v < num_names; v++) {
        q[v] = d[v] = MAX_NUM;
    }

    /* erdos가 몇번인지 찾기*/
    for (v = 0; v < num_names; v++) {
        if (strcmp(names[v], "Erdos, P.") == 0) {
            s = v;
            /*printf("s = %d\n", s);*/
            break;
        }
    }
    q[s] = d[s] = 0;

    while (!q_empty()) {
        u = q_extract_min();
        for (v = 0; v < num_names; v++) {
            if (graph[u][v] > 0) {
                if (d[v] > d[u] + graph[u][v]) {
                    /*printf("d[%d] > d[%d] + graph[%d][%d]\n", v, u, u, v);*/
                    d[v] = d[u] + graph[u][v];
                    q[v] = d[v];
                    /*printf("d[%d] = %d\n", v, d[v]);*/
                }
            }
        }
    }
}

void output(int scenario)
{
    int i;
    printf("Scenario %d\n", scenario);
    for (i = 0; authors[i] >= 0; i++) {
        printf("%s ", names[authors[i]]);
        if (d[authors[i]] == MAX_NUM) {
            printf("infinity\n");
        } else {
            printf("%d\n", d[authors[i]]);
        }
    }
}


int main()
{
    int i;
    scanf("%d", &num_scenario);

    for (i = 1; i <= num_scenario; i++) {
        input();
        process();
        output(i);
    }
}

/* vim: set ts=4 sw=4 : */
