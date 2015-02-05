#include <stdio.h>
#include <string.h>

char a[11];
char b[11];
int c;

int input()
{
    int len, i;
    char *pc;

    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));

    scanf("%s %s", a, b);

    if (a[0] == '0' && b[0] == '0')
        return 0;

    len = strlen(a);
    for (i=10, pc = &a[len-1]; pc >= a; pc--,i--) {
        a[i] = *pc - '0';
    }
    while (i >= 0)
        a[i--] = 0;


    len = strlen(b);
    for (i=10, pc = &b[len-1]; pc >= b; pc--,i--) {
        b[i] = *pc - '0';
    }
    while (i >= 0)
        b[i--] = 0;

    return 1;
}

int process()
{
    int i;
    int c = 0;
    int cnt = 0;
    int s;

    for (i=10; i>=0; i--) {
        s = a[i] + b[i] + c;
        c = s/10;
        s = s%10;
        a[i] = s;

        cnt += c;
    }

    return cnt;
}

int main()
{
    int c;
    while (1) {
        if (!input())
            break;

        c = process();
        if (c == 0)
            printf("No ");
        else
            printf("%d ", c);
        printf("carry operation%s.\n", c > 1 ? "s" : "");
    }

    return 0;
}
