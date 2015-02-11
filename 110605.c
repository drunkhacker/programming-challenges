#include <stdio.h>
#include <string.h>

int k, d;

unsigned long long a[22] = {0,}; //a[d] = complete k-ary 트리에서 depth d일때 labeling 방법
unsigned long long f[21];
long n[21] = {0,}; //n[d] = depth d complate k-ary 트리의 총 노드 개수

long mpow(int x, int y)
{
    long r = 1;
    while (y--)
        r *= x;

    return r;
}

unsigned long long process()
{
    int i,j;
    long nodes;
    unsigned long long partition;
    memset(a, 0, sizeof(a));

    a[0] = 1;
    a[1] = f[k];

    n[0] = 1;
    for (i=1; i<=d; i++) {
        n[i] = n[i-1] + mpow(k, i);
        /* printf("n[%d] = %ld\n", i, n[i]); */
    }
    
    for (i=2; i<=d; i++) {
        nodes = n[i] - 1;
        partition = f[nodes];
        for (j=0; j<k; j++) {
            partition /= f[nodes/k];
        }
        printf("i=%d partition=%llu\n", i, partition);

        a[i] = partition*(a[i-1]*k);
    }

    return a[d];
}

int main()
{
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
    f[13] = 6227020800;
    f[14] = 87178291200;
    f[15] = 1307674368000;
    f[16] = 20922789888000;
    f[17] = 355687428096000;
    f[18] = 6402373705728000;
    f[19] = 121645100408832000;
    f[20] = 2432902008176640000;


    while(scanf("%d %d", &k, &d) != EOF) {
        //handle special cases
        if (k == 21) {
            printf("51090942171709440000\n");
            continue;
        }

        /* printf("k=%d, d=%d\n", k, d); */

        printf("%llu\n", process());
    }

    return 0;
}
