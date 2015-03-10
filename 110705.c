#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_N 10000000

int primes[670000];
int A[MAX_N];
int n_primes = 0;

int pre_calc_prime()
{
    // Sieve of Eratosthenes
    int n, i, j;
    int prime = 0;

    memset(A, 0, sizeof(A));

    for (i = 2; i < sqrt(MAX_N); i++) {
        if (!A[i]) {
            for (j = i*i; j < MAX_N; j += i) {
                A[j] = 1;
            }
        }
    }

    for (i = 2; i < MAX_N; i++) {
        if (!A[i]) {
            primes[n_primes++] = i;
        }
    }
}

int is_prime(int n)
{
    int i;
    int imax = n_primes - 1;
    int imin = 0;
    int imid;

    //binary search
    while (imax >= imin) {
        imid = imin + (imax-imin)/2;
        if (primes[imid] == n) {
            return 1;
        } else if (primes[imid] < n) {
            imin = imid + 1;
        } else if (primes[imid] > n) {
            imax = imid - 1;
        }
    }

    return 0;
}

void goldbach(int n)
{
    int i;
    int p;

    /* printf("n = %d\n", n);
    if (n % 2) {
        fprintf(stderr, "n = %d!\n", n);
        exit(1);
    } */


    for (i = n_primes-1; i >= 0; i--) {
        if (primes[i] <= n) {
            break;
        }
    }

    for (; i >= 0; i--) {
        p = n - primes[i];
        if (is_prime(p)) {
            printf("%d %d ", primes[i], p);
            return;
        }
    }
}

/* void waring(int n)
{
    int i, j;
    int p;

    for (i = n_primes-1; i >= 0; i--) {
        if (primes[i] <= n-4) {
            break;
        }
    }

} */

void process(int n)
{
    int i;
    int p;

    if (n <= 7) {
        printf("Impossible.\n");
        return;
    }

    // 골드바흐의 추측 : 
    // 짝수는 두 소수의 합이다
    //
    // Waring의 추측 :
    // 홀수는 세 소수의 합이거나 자기자신이 소수다.

    if (n % 2 == 0) {
        if ((n/2)%2 == 0) {
            goldbach(n/2);
            goldbach(n/2);
        } else {
            //n/2 is odd
            printf("3 3 ");
            goldbach(n-6);
        }
    } else {
        /* for (i = n_primes-1; i >= 0; i--) {
            if (primes[i] <= n-6) {
                p = primes[i];
                break;
            }
        } */
        printf("2 3 ");
        goldbach(n-5);
    }

    printf("\n");
}

int main()
{
    int n;

    //pre-populate primes
    pre_calc_prime();

    while (scanf("%d", &n) != EOF) {
        process(n);
    }
}
