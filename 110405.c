#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

int fines[MAX_N];
int times[MAX_N];
int jobs[MAX_N];
int sum_time;
int sum_fine;

int order_len;

int metric(int idx) {
    return (sum_fine - fines[idx])*times[idx];
}
int compare(const void *p1, const void *p2)
{
    int idx1 = *(int *)p1 - 1;
    int idx2 = *(int *)p2 - 1;

    return metric(idx1) - metric(idx2);
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

void process_range(int st, int en)
{
    int i;

    sum_fine = 0;
    for (i=st; i<en; i++) {
        sum_fine += fines[jobs[i]-1];
    }

    qsort(&jobs[st], en-st, sizeof(int), compare);
}

void process()
{
    int st = 0;
    int t;
    int i;

    for (st = 0; st < order_len-1; st++) {
        process_range(st, order_len);
#if 0
        printf("range=%d-%d\n", st, order_len);

        for (i=st; i<order_len; i++) {
            printf("%d %d: %d\n", times[i], fines[i], metric(i));
        }

        printf("jobs[st] = %d\n", jobs[st]);
#endif

        if (compare(&jobs[st], &jobs[st+1]) == 0 && jobs[st] > jobs[st+1]) {
            t = jobs[st];
            jobs[st] = jobs[st+1];
            jobs[st+1] = t;
        }
    }
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
