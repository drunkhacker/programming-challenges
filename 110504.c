#include <stdio.h>

int k[10][10] = {0,}; // k[i][j]의 의미: 끝자리가 i로 끝나는 수에 k[i][j]를 곱해야 끝자리가 j인 수가 나온다

/* 알고리즘
 * ex) 9901
 *
 * 0. s <- 0, j <- 1
 * 1. s += a*k[a[0]][j]  ( s = 9901 )
 *    finish if a*k[a[0]][j] == 'all 1'
 * 2. s /= 10            ( s = 990 )
 * 3. j = (11-s%10)%10   ( j = 1 )
 *    goto 1
 */

#define a0 ((a)%10)
#define s0 ((s)%10)

int all1(int t)
{
    int c = 0;
    for (; t>0; t/=10) {
        if (t % 10 != 1)
            return 0;
        c++;
    }
    return c;
}

int process(int a)
{
    int s = 0, j = 1;
    int t, c = 0; // c: counter of 1

    while (1) {
        if (all1(s) > 0)
            return c + all1(s);

        t = a*k[a0][j];

        /*printf("%d", k[a0][j]);*/

        s += t;
        s /= 10;

        j = (11 - s0) % 10;
        /*printf("s=%d, j=%d\n", s, j);*/
        c++;
    }

    return c;
}

int main()
{
    int a, i, j, m;

    //initialize k[]
    for (i=1; i<=9; i++) {
        if (i == 2 || i == 5) continue;

        for (j=1; j<=9; j++) {
            for (m=1; m<=9; m++) {
                if ((i*m)%10 == j) {
                    k[i][j] = m;
                    break;
                }
            }

            /*printf("k[%d][%d] = %d\n", i,j,k[i][j]);*/
        }
    }

    while(scanf("%d", &a) != EOF) {
        if ((j=all1(a)) > 0) {
            printf("%d\n", j);
            continue;
        }
        /*printf("%dx", a);*/
        process(a);
        /*printf("\n");*/
        printf("%d\n", process(a));
    }

    return 0;
}
