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

void input()
{
    int i, j = 0, p, n, num_authors, m;
    char buf[BUFSIZ];
    char *pc;
    char *paper;
    char *name;
    int num_names = 0;

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
        printf("paper name: %s\n", paper);
        for (i = 0; i < num_authors; i++) {
            printf("--%s(%d)\n", names[authors[i]], authors[i]);
        }

        /* author들끼리 연관지어주기*/
        for (i = 0; i < num_authors - 1; i++) {
            for (j = i + 1; j < num_authors; j++) {
                graph[authors[i]][authors[j]]++;
                graph[authors[j]][authors[i]]++;
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

    /* debug output of graph */
    for (i = 0; i < num_names; i++) {
        for (j = 0; j < num_names; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}

void process()
{
    int v, s;
    memset(d, 0, 100*sizeof(int));

    for (v = 0; v < num_names; v++) {
        d[v] = 0;
    }

    /* erdos가 몇번인지 찾기*/
    for (v = 0; v < num_names; v++) {
        if (strcmp(names[v], "Erdos, P.") == 0) {
            s = v;
            break;
        }
    }
    d[s] = 0;
}

void output()
{
}


int main()
{
    int i;
    scanf("%d", &num_scenario);

    for (i=0; i<num_scenario; i++) {
        input();
        process();
        output();
    }
}

/* vim: set ts=4 sw=4 : */
