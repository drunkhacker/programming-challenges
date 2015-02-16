#include <stdio.h>

/* �˰���
 * f(1) f(2) f(3) f(4) f(5) f(6) f(7) f(8) .....
 *   1   2    2    3    3    4    4    4  .........
 *
 * f(n) ���� �ٲ�� n�� �þ���� ������ a(k)�̶� ����
 *
 * a(1) a(2) a(3) a(4) ...
 *  1    2    4    6   ...
 *
 *  a(4)�� f(6)�� 4�� �ȴٴ� ���̰�, 6��°�� ������ ������ 4�� f(4)�� �� ���ð�.
 *  ��, ���� f(k)���� �ٲ���� f(4)�� �� ���;� �Ѵٴ� ��
 *  ���� a(5) = a(4) + f(4)
 *  ��, a(k+1) = a(k) + f(k)
 *
 *  �׸��� �� �鿩�ٺ���,
 *  f(a(k)) ���� f(a(k+1) - 1) ���� ��� ���� k���� �� ���ִ�.
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

