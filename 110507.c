#include <stdio.h>

int n, m;

/* 알고리즘
 * Stern-Brocot 트리에서 현재 노드가 c_m/c_n 이고 부모노드가 p_m/p_n, 다른 부모노드는 pp_m/pp_n 이라 하자
 * pp_m = c_m - p_m, pp_n = c_n - p_n 이 성립.
 *
 * 이때 탐색할 다음노드를 정하는 케이스를 나누면
 * A) m/n > c_m/c_n -> 오른쪽으로
 * A-1) p_m/p_n > c_m/c_n 
 *      c_m = p_m + c_m
 *      c_n = p_n + c_n
 * A-2) p_m/p_n < c_m/c_n
 *      c_m = pp_m + c_m
 *      c_n = pp_n + c_n
 *
 * B) m/n < c_m/c_n -> 왼쪽으로
 * B-1) p_m/p_n > c_m/c_n 
 *      c_m = pp_m + c_m
 *      c_n = pp_n + c_n
 * B-2) p_m/p_n < c_m/c_n
 *      c_m = p_m + c_m
 *      c_n = p_n + c_n
 *
 * c_m == m, c_n == n 일때까지 계속
 */

void compute(int c_m, int c_n, int p_m, int p_n)
{
    int pp_m, pp_n;
    int t;
    while (1) {
        pp_m = c_m - p_m;
        pp_n = c_n - p_n;

        if (m == c_m && n == c_n)
            return;

        if (m*c_n > n*c_m) {
            //case A
            putchar('R');
            if (p_m*c_n > p_n*c_m) {
                //case A-1
                pp_m = c_m;
                pp_n = c_n;

                c_m += p_m;
                c_n += p_n;

                p_m = pp_m;
                p_n = pp_n;
            } else {
                //case A-2
                p_m = c_m;
                p_n = c_n;

                c_m += pp_m;
                c_n += pp_n;
            }

        } else {
            //case B
            putchar('L');
            if (p_m*c_n > p_n*c_m) {
                //case B-1
                p_m = c_m;
                p_n = c_n;

                c_m += pp_m;
                c_n += pp_n;
            } else {
                //case B-2
                pp_m = c_m;
                pp_n = c_n;

                c_m += p_m;
                c_n += p_n;

                p_m = pp_m;
                p_n = pp_n;
            }
        }
    }
}

void process()
{
    // 첫번째로 탐색할 노드를 정하자
    // m/n > 1 이면 오른쪽 트리로 -> 2/1 이 첫 노드
    // m/n < 1 이면 왼쪽 트리로 -> 1/2 

    if (m > n) {
        putchar('R');
        compute(2, 1, 1, 1);
    } else if (m < n) {
        putchar('L');
        compute(1, 2, 1, 1);
    }
}

int main()
{
    while (1) {
        scanf("%d%d", &m, &n); // m/n
        if (m == 1 && n == 1)
            break;
        process();
        putchar('\n');
    }

    return 0;
}
