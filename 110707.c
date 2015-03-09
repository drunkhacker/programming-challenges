#include <stdio.h>

/* 알고리즘 스케치
 * c1m1 + c2m2 = k 
 * n1m1 + n2m2 = n 의 격자점 해를 찾는 문제.
 *
 * 따라서, k의 탐색범위를 구한 뒤,
 * 그 범위안의 c1*km1 + c2*km2 = gcd(c1,c2)을 만족하는 k를 탐색하면서
 * 1) m1, m2가 0이상이며
 * 2) n1m1 + n2m2 = n 위에 있는지 확인
 *
 * see http://math.stackexchange.com/questions/32127/does-ax-by-c-pass-through-any-lattice-point
 */

/* p*x + q*y = gcd(p, q) */
long gcd(long p, long q, long *x, long *y)
{
    long x1, y1;
    long g;

    if (q > p) return gcd(q, p, y, x);

    if (q == 0) {
        *x = 1;
        *y = 0;
        return p;
    }

    g = gcd(q, p%q, &x1, &y1);

    *x = y1;
    *y = (x1 - (p/q)*y1);

    return g;
}

void process(int n, int c1, int n1, int c2, int n2, int *m1, int *m2)
{
    long k;
    long g;
    long x, y; //x -> m1, y -> m2
    long dx, dy;
    int i;

    g = gcd(n1, n2, &x, &y);
    printf("%d*%ld + %d*%ld = %ld\n", n1, x, n2, y, g);

    if (n % g != 0) {
        *m1 = -1;
        *m2 = -1;
        return;
    }

    x *= n/g;
    y *= n/g;
    i = x - y > 0;

    // x나 y중 둘 중 하나는 음수일것
    dx = n2/g * ((x <= 0) ? 1 : -1);
    dy = n1/g * ((x <= 0) ? -1 : 1);

    printf("dx = %ld, dy = %ld\n", dx, dy);

    printf("x = %ld, y = %ld\n", x, y);
    while ((x < 0 || y < 0) && i == (x - y > 0)) {
        x += dx;
        y += dy;
    }
    printf("x = %ld, y = %ld\n", x, y);

    *m1 = -1;
    *m2 = -1;
    k = ((long)1<<63) - 1;
    while (x >= 0 && y >= 0) {
        if (k > c1*x + c2*y) {
            k = c1*x + c2*y;
            *m1 = x;
            *m2 = y;

            /*printf("x=%ld, y=%ld, k=%ld\n", x, y, k);*/
        }

        x += dx;
        y += dy;
    }
}

int main()
{
    int n, c1, n1, c2, n2, m1, m2;
    while (1) {
        scanf("%d", &n);
        if (n == 0) break;
        scanf("%d%d", &c1, &n1);
        scanf("%d%d", &c2, &n2);

        process(n, c1, n1, c2, n2, &m1, &m2);

        if (m1 >= 0 && m2 >= 0) {
            printf("%d %d\n", m1, m2);
        } else {
            printf("failed\n");
        }
    }

    return 0;
}
