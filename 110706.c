#include <stdio.h>
#include <math.h>

int sum_digits(int k)
{
    int s = 0;

    while (k > 0) {
        s += k % 10;
        k /= 10;
    }

    return s;
}

int sum_factors(int n)
{
    int s = 0;
    int p = 3;

    while (n % 2 == 0) {
        n /= 2;
        s += 2;
        /* printf("%d ", 2); */
    }


    for (p = 3; p <= sqrt(n) + 1;) {
        if (n % p == 0) {
            s += sum_digits(p);
            n /= p;
            /* printf("%d ", p); */
        } else {
            p += 2;
        }
    }

    if (n > 1) {
        s += sum_digits(n);
        /* printf("%d", n); */
    }

    /* printf("\n"); */

    return s;
}

int is_prime(int n)
{
    int i;

    if (n == 2) 
        return 1;

    for (i = 2; i < sqrt(n)+1; i++) {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int smith_number(int k)
{
    int n;

    for (n = k+1; ;n++) {
        if (!is_prime(n) && sum_digits(n) == sum_factors(n))
            return n;
    }
}

int main()
{
    int n, i, k;

    /* test */
    /* printf("%d\n", sum_factors(4997801)); */

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &k);
        printf("%d\n", smith_number(k));
    }

    return 0;
}
