#include <stdio.h>

unsigned long long n;
int iterate;
unsigned long long sum;

unsigned long long getd(unsigned long long n)
{
    unsigned long long d = 1;
    unsigned long long n2 = n;

    while (n2 >= 10) {
        d *= 10;
        n2 /= 10;
    }

    return d;
}

int palindrome(unsigned long long n)
{
    unsigned long long d;

    if (n < 10) {
        return 1;
    }

    d = getd(n);

    while (n >= 10) {
        if (n/d == n%10) {
            n -= (n/d)*d;
            n /= 10;
            d /= 100;
            /* printf("n = %d\n", n); */
        } else
            return 0;
    }

    return 1;
}

void process()
{
    unsigned long long d = 1;
    unsigned long long n2 = n;

    iterate = 0;
    sum = n;

    if (palindrome(sum))
        return;

    while (1) {
        d = getd(sum);

        while (n2 > 0) {
            sum += (n2%10)*d;
            n2 /= 10;
            d /= 10;

            /* printf("n2=%llu, d=%llu, sum=%llu\n", n2, d, sum); */
        }
        iterate++;

        if (palindrome(sum))
            return;

        n = sum;
        n2 = sum;
    }
}

int main()
{
    int cases;

    /* printf("%d\n", palindrome(11)); */

    scanf("%d", &cases);
    while (cases--) {
        scanf("%llu", &n);
        process();
        printf("%d %llu\n", iterate, sum);
    }

    return 0;
}
