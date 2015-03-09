#include <stdio.h>

/* 알고리즘 스케치
 * n1x + n2y = n (n직선) 의 x>=0, y>=0 격자점 중에서
 * c1x + c2y = k (c직선) 를 최소화하는 경우를 찾는 문제
 *
 * 기울기의 관계에 따라 경우가 나뉘는데,
 * 1) c직선이 n직선보다 가파른 경우
 *    n직선의 x절편 근처의 첫번째 격자점이 답
 * 2) c직선이 n직선보다 완만한 경우
 *    n직선의 y절편 근처의 첫번째 격자점이 답
 *
 * euclid alg.을 써서 n1x + n2y = g = gcd(n1, n2)를 만족하는 x,y를 구한 다음에
 * n/g를 양변에 곱해서 n직선 위의 점 (X, Y)를 구한다.
 * 이때 다른 격자점들은 dx = n2, dy = n1를 각각 X, Y에 더하거나 빼거나 하면 나옴.
 *
 * 위에서 나눈 경우에 따라 절편 근처의 격자점들을 찾으면 됨.
 * X 및 Y의 대소관계에 따라 dx, dy의 부호를 결정해서 X>=0, Y>=0 이 되기위해 dx,dy를 얼마나
 * 더하거나 빼야 하는지 알아내면 됨.
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

#define CEIL_DIV(x, n) (((x)+(n)-1)/(n))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

void process(int n, int c1, int n1, int c2, int n2, int *m1, int *m2)
{
    long k;
    long g;
    long x, y; //x -> m1, y -> m2
    long dx, dy;
    int i;

    g = gcd(n1, n2, &x, &y);
    /*printf("%d*%ld + %d*%ld = %ld\n", n1, x, n2, y, g);*/

    if (n % g != 0) {
        *m1 = -1;
        *m2 = -1;
        return;
    }

    x *= n/g;
    y *= n/g;

    dx = n2/g;
    dy = n1/g;

    if ((long double)c1/c2 > (long double)n1/n2) {
        if (y > x) {
            /*printf("case1\n");*/
            // x < 0, y > 0
            k = CEIL_DIV(-x, dx);
            dy *= -1;
        } else {
            /*printf("case2\n");*/
            // x > 0, y < 0
            k = x/dx;
            dx *= -1;
        }
    } else {
        if (y > x) {
            /*printf("case3\n");*/
            // x < 0, y > 0
            k = y/dy;
            dy *= -1;
        } else {
            /*printf("case4\n");*/
            // x > 0, y < 0
            k = CEIL_DIV(-y, dy);
            dx *= -1;
        }
    }

    x += k*dx;
    y += k*dy;

    /*printf("dx = %ld, dy = %ld, k = %ld\n", dx, dy, k);*/
    /*printf("x = %ld, y = %ld\n", x, y);*/

    *m1 = x;
    *m2 = y;
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
