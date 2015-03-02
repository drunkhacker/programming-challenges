#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

int fines[MAX_N];
int times[MAX_N];
int jobs[MAX_N];

int order_len;

int compare(const void *p1, const void *p2)
{
    int idx1 = *(int *)p1 - 1;
    int idx2 = *(int *)p2 - 1;

    return fines[idx2]*times[idx1] - fines[idx1]*times[idx2];
}

void input()
{
    int i;
    /*printf("input\n");*/
    scanf("%d", &order_len);

    for (i=0; i<order_len; i++) {
        scanf("%d%d", &times[i], &fines[i]);
        jobs[i] = i+1;
    }
    if (getchar() == EOF)
        return;
    getchar();//newline
}

void process()
{
    int st = 0;
    int t;
    int i;

    qsort(jobs, order_len, sizeof(int), compare);
}

void output()
{
    int i;
    for (i=0; i<order_len-1; i++) {
        printf("%d ", jobs[i]);
    }
    printf("%d\n", jobs[i]);
}

int main()
{
    int cases, i, first = 1;

    scanf("%d", &cases);
    getchar();
    getchar();//newline

    for (i=0; i<cases; i++) {
        input();
        process();
        if (first) {
            first = 0;
        } else {
            printf("\n");
        }
        output();
    }

    return 0;
}
