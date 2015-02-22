#include <stdio.h>
#include <stdlib.h>

#define abs(a) ((a)>0?(a):(-(a)))

int street[500] = {0,};

int compare(const void *p1, const void *p2)
{
    return *(int *)p1 - *(int *)p2;
}

int main()
{
    int cases, i, j, n, s;

    scanf("%d", &cases);

    for (i=0; i<cases; i++) {
        scanf("%d", &n);
        for (j=0; j<n; j++)
            scanf("%d", &street[j]);

        qsort(street, n, sizeof(int), compare);

        s = 0;
        for (j=0; j<n; j++)
            s += abs(street[n/2] - street[j]);

        printf("%d\n", s);
    }

    return 0;
}
