#include <stdio.h>
#include <math.h>

#define MAX_N 65536

int divisor_count[MAX_N] = {0,};

void count_divisor(int n)
{
    int i = 2;
    for (i = 2; i < n; i++) {
        divisor_count[n] += n % i == 0;
    }
    divisor_count[n] += 2;
}

int process(unsigned int n)
{
    int root_n = (int)sqrt(n);

    if (n < MAX_N)
        return divisor_count[n] % 2 == 1;
    else
        return root_n*root_n == n;
}

int main()
{
    unsigned int n;
    int i;

    for (i = 1; i < MAX_N; i++) {
        count_divisor(i);
    }

    /* test */
    /* printf("16 has %d divisors\n", divisor_count[16]); */

    while (1) {
        scanf("%d", &n);

        if (n == 0)
            break;

        printf("%s\n", process(n) ? "yes" : "no");
    }

    return 0;
}

