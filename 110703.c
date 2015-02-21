#include <stdio.h>

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

int main()
{
    long a, b, x, y, g;

    while (scanf("%ld%ld", &a, &b) != EOF) {
        g = gcd(a, b, &x, &y);
        printf("%ld %ld %ld\n", x, y, g);
    }

    return 0;
}
