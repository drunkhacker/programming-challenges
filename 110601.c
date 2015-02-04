#include <stdio.h>
#include <string.h>

char fib[500][150] = {0,}; //fib[x][M] : MSB, fib[x][149] : LSB
int fib_digits[500] = {0,};

void print(char *a)
{
    int i;
    for (i = 0; i < 150; i++) {
        if (a[i] != 0) {
            break;
        }
    }

    while (i < 150) {
        printf("%d", a[i++]);
    }
}

int digit(char *a)
{
    int i;
    for (i = 0; i < 150; i++) {
        if (a[i] != 0) {
            return 150 - i;
        }
    }
    return 0;
}

void add(char *a, char *b, char *r)
{
    int c = 0; //carry
    int s; //digit sum
    int i;

    for (i = 149; i >= 0; i--) {
        s = a[i] + b[i] + c;
        c = s / 10;
        s = s % 10;
        r[i] = s;
    }
}

void make_fib()
{
    int i = 3;

    memset(fib, 0, sizeof(fib));

    fib[1][149] = 1;
    fib[2][149] = 1;

    printf("f(1) = ");
    print(fib[1]); fputc('\n', stdout);
    printf("f(2) = ");
    print(fib[2]); fputc('\n', stdout);

    while (1) {
        add(fib[i-2], fib[i-1], fib[i]);
        printf("f(%d) = ", i);
        print(fib[i]); fputc('\n', stdout);

        fib_digits[i] = digit(fib[i]);

        if (digit(fib[i]) > 100) break;
        i++;
    }
}

int main()
{
    char a[150];
    char b[150];
    int  i;

    // make fib
    make_fib();

    while (1) {
        scanf("%s %s", a, b);

        if (a[0] == '0' && b[0] == '0') 
            break;

    }

    return 0;
}

/* vim: set ts=4 sw=4 : */
