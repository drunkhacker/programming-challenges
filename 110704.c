#include <stdio.h>
#include <math.h>

int isprime(unsigned long x)
{
    unsigned long i = 3;

    if (x % 2 == 0) return 0;

    while (i <= sqrt(x) + 1) {
        if (x % i == 0) return 0;
        i += 2;
    }

    return 1;
}

int process(unsigned long n, unsigned long m)
{
    unsigned long n2;
    unsigned long i;

    /*printf("n=%ld, m=%ld\n", n, m);*/

    while (n > 0 && m > 1) {
        n2 = n;
        /*printf("%ld: ", n2);*/
        if (n2 >= m)
            return 1;
        //find factors of n2
        while ((n2 % 2) == 0) {
            if (m % 2 == 0) {
                m /= 2;
                if (m == 1) return 1;
            }
            n2 /= 2;
            /*printf("2 ");*/
        }

        i = 3;
        while (i <= sqrt(n2) + 1) {
            if (n2 % i == 0) {
                if (m % i == 0) {
                    m /= i;
                    if (m == 1) return 1;
                }
                n2 /= i;
                /*printf("%ld ", i);*/
            } else {
                i += 2;
            }
        }

        if (n2 > 1) {
            if (m % n2 == 0) {
                m /= n2;
                if (m == 1) return 1;
            }
            /*printf("%ld ", n2);*/
        }
        /*printf(", m=%ld\n", m);*/

        n--;
    }

    /*printf("m = %ld\n", m);*/

    return m == 1;
}

int main()
{
    unsigned long n, m;

    while (scanf("%ld%ld", &n, &m) != EOF) {
        if ((m <= n && m > 0) || m == 1) {
            printf("%ld divides %ld!\n", m, n);
        } else {
            printf("%ld %s %ld!\n", m, m > 0 && !isprime(m) && process(n, m) ? "divides" : "does not divide", n);
        }
    }

    return 0;
}
