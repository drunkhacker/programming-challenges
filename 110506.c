#include <stdio.h>

int n, k, c;
int t[12];
int f[13] = {0,};

void process() {
    int i;
    int s = 0;

    for (i=0; i<k; i++)
        s += t[i];

    c = f[s];
    for (i=0; i<k; i++)
        if (t[i] > 0)
            c /= f[t[i]];
}

int main()
{
    int i;

    f[1] = 1;
    f[2] = 2;
    f[3] = 6;
    f[4] = 24;
    f[5] = 120;
    f[6] = 720;
    f[7] = 5040;
    f[8] = 40320;
    f[9] = 362880;
    f[10] = 3628800;
    f[11] = 39916800;
    f[12] = 479001600;

    while(1) {
        if (scanf("%d %d", &n, &k) == EOF) break;
        for (i=0; i<k; i++) {
            scanf("%d", &t[i]);
        }
        process();
        printf("%d\n", c);
    }

    return 0;
}
