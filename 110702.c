#include <stdio.h>
#include <math.h>

#define MAX_N 65000

int is_prime[MAX_N];

int mod_exp(unsigned int a, unsigned int n, int m)
{
    int s = 1;
    //calc a^n mod m
    while (n > 0) {
        if (n % 2) {
            s = s*a % m;
            n -= 1;
        }

        a = a*a % m;
        n /= 2;
    }

    return s;
}

int carmichael(int n)
{
    int i, k;

    if (is_prime[n])
        return 0;

    for (i = 2; i < n; i++) {
        if (mod_exp(i, n, n) != i) {
            /* debug */
            /* printf("%d^%d (mod %d) = %d\n", i, n, n, mod_exp(i, n)); */
            return 0;
        }
    }

    return 1;
}

void pre_calc_prime()
{
    int n, i;

    is_prime[1] = 0;
    is_prime[2] = 1;

    for (n = 3; n < MAX_N; n++)  {
        is_prime[n] = 1;
        for (i = 2; i < sqrt(n)+1; i++) {
            if (n % i == 0) {
                is_prime[n] = 0;
                break;
            }
        }
    }
}

int main()
{
    int n;

    /* mod_exp test */
    /* printf("1924^3800 (mod 3800) = %d\n", mod_exp(1924, 3800)); */

    pre_calc_prime();

    while (1) {
        scanf("%d", &n);

        if (n == 0)
            break;

        if (carmichael(n))
            printf("The number %d is a Carmichael number.\n", n);
        else
            printf("%d is normal.\n", n);
    }

    return 0;
}
