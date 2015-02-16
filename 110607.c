#include <stdio.h>

/* 알고리즘
 * f(1) f(2) f(3) f(4) f(5) f(6) f(7) f(8) .....
 *   1   2    2    3    3    4    4    4  .........
 *
 * f(n) 값이 바뀌는 n을 늘어놓은 수열을 a(k)이라 하자
 *
 * a(1) a(2) a(3) a(4) ...
 *  1    2    4    6   ...
 *
 *  a(4)는 f(6)이 4가 된다는 뜻이고, 6번째항 포함해 앞으로 4는 f(4)번 더 나올것.
 *  즉, 다음 f(k)값이 바뀔려면 f(4)번 더 나와야 한다는 뜻
 *  따라서 a(5) = a(4) + f(4)
 *  즉, a(k+1) = a(k) + f(k)
 *
 *  그리고 잘 들여다보면,
 *  f(a(k)) 부터 f(a(k+1) - 1) 까지 모두 값이 k임을 알 수있다.
 */

#define MAX_A 700000
int a[MAX_A+1];

int f(int k)
{
    int i;
    for (i=1; i<MAX_A; i++) {
        if (k >= a[i] && a[i+1] > k) {
            /* printf("f(%d)=%d\n", k, i); */
            return i;
        }

    }

    /* printf("error...k = %d\n", k); */

    return 0;
}

int main()
{
    //pre-populate
    int i, n;
    a[1] = 1;
    a[2] = 2;
    a[3] = 4;
    for (i=3; i<MAX_A; i++) {
        a[i+1] = a[i] + f(i);
    }

    /* for (i=1; i<1000000; i++) { */
        /* printf("a[%d] = %d\n", i, a[i]); */
    /* } */
    /* printf("a[%d] = %d\n", MAX_A, a[MAX_A]); */


    while(1) {
        scanf("%d", &n);
        if (n == 0) break;
        printf("%d\n", f(n));
    }

    return 0;
}

