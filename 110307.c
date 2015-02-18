#include <stdio.h>
#include <string.h>

#define MAX_N 25150
#define MAXINT 999999

char *dict[MAX_N];
int dict_size;

char str1[17];
char str2[17];
int index1;
int index2;

char graph[MAX_N][MAX_N] = {0,};
int d[MAX_N];
char q[MAX_N];
int prev[MAX_N];

int doublet(char *str1, char *str2)
{
    int c = 0;
    int i;

    if (strlen(str1) != strlen(str2)) return 0;

    for (i=0; i<strlen(str1); i++) {
        c += str1[i] != str2[i];
        if (c > 1) return 0;
    }

    return c == 1;
}

void input_dict()
{
    char buf[BUFSIZ];
    int i,j;

    memset(dict, 0, sizeof(dict));
    memset(graph, 0, sizeof(graph));
    dict_size = 0;

    while (1) {
        fgets(buf, BUFSIZ-1, stdin);
        if (buf[0] == '\n')
            break;
        buf[strlen(buf)-1] = 0;

        dict[dict_size++] = strdup(buf);
    }

    //make graph
    for (i=0; i<dict_size-1; i++)  {
        for (j=i+1; j<dict_size; j++) {
            if (doublet(dict[i], dict[j]))
                graph[i][j] = graph[j][i] = 1;
        }
    }
}

void dijkstra(int start, int finish)
{
    int i;
    int v;
    int min_d;

    for (i=0; i<dict_size; i++) {
        d[i] = MAXINT;
        q[i] = 0;
    }
    d[start] = 0;
    v = start;

    while (!q[v]) {
        q[v] = 1;
        if (v == finish)
            return;

        for (i=0; i<dict_size; i++) {
            if (graph[v][i] && d[i] > d[v] + graph[v][i]) {
                d[i] = d[v] + graph[v][i];
                prev[i] = v;
            }
        }

        v = 0;
        min_d = MAXINT;
        for (i=0; i<dict_size; i++) {
            if (!q[i] && min_d > d[i]) {
                min_d = d[i];
                v = i;
            }
        }
    }
}

void process()
{
    int i;
    int seq_len;

    for (i=0; i<dict_size; i++)
        prev[i] = -1;

    //find str1, str2 pos
    index1 = index2 = -1;
    for (i=0; i<dict_size; i++) {
        if (strcmp(dict[i], str1) == 0)
            index1 = i;
        if (strcmp(dict[i], str2) == 0)
            index2 = i;
    }

    if (index1 >= 0 && index2 >= 0) {
        if (index1 == index2) {
            for (i=0; i<dict_size; i++) {
                if (graph[index2][i]) {
                    prev[index2] = i;
                    break;
                }
            }
        } else {
            dijkstra(index1, index2);
        }
    }
}

void output()
{
    int v = index2;
    int seq[MAX_N];
    int i = 0;

    if (index1 < 0 || index2 < 0) {
        printf("No solution.\n");
        return;
    }

    while (prev[v] >= 0) {
        seq[i++] = v;
        v = prev[v];
    }

    if (i == 0) {
        printf("No solution.\n");
    } else {
        seq[i++] = v;
        if (index1 == index2)
            printf("%s\n", dict[index1]);

        while (i--) {
            printf("%s\n", dict[seq[i]]);
        }
    }
}

int main()
{
    int first = 1;

    input_dict();
    /*printf("hi\n");*/

    while(1) {
        if (scanf("%s%s", str1, str2) == EOF)
            break;

        if (!first)
            printf("\n");
        else
            first = 0;

        if (strlen(str1) != strlen(str2)) {
            printf("No solution.\n");
            continue;
        }

        process();
        output();
    }

    return 0;
}
