#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n;
int a[50];
int b[10];

int input()
{
    int i;

    memset(a, 0, sizeof(a));

    if (scanf("%d", &n) == EOF)
        return 0;

    for (i=0; i<n*(n-1)/2; i++) {
        if (scanf("%d", &a[i]) == EOF)
            return 0;
    }

    return 1;
}

int compare(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}


int process()
{
    int sum = 0;
    int i, j = 0;

    memset(b, 0, sizeof(b));

    for (i=0; i<n*(n-1)/2; i++)
        sum += a[i];

    if (sum%(n-1))
        return 0;
    sum /= n-1;

    for (i=0; i<n*(n-1)/2; i++) {
        b[j++] = sum-a[i];
        /* printf("b[%d]=%d\n", j-1, b[j-1]); */
    }

    /* printf("j=%d\n", j); */

    qsort(b, n, sizeof(int), compare);

    return 1;
}

void output()
{
    int i;

    for (i=0; i<n; i++) 
        printf("%d%c", b[i], i == n-1 ? '\n' : ' ');
}


int main()
{
    while (1) {
        if (!input())
            break;
        if (process())
            output();
        else
            printf("Impossible\n");
    }
    return 0;
}
