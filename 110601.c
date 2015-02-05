#include <stdio.h>
#include <string.h>

char fib[500][150] = {0,}; //fib[x][M] : MSB, fib[x][149] : LSB
int fib_digits[500] = {0,};

int compare(char *a, char *b)
{
    int i;
    for (i=0; i<150; i++) {
        if (a[i] < b[i]) 
            return -1;
        else if (a[i] == b[i]) 
            continue;
        else 
            return 1;
    }

    return 0;
}

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
    fib[2][149] = 2;
    fib_digits[1] = 1;
    fib_digits[2] = 1;

    /* printf("f(1) = "); */
    /* print(fib[1]); fputc('\n', stdout); */
    /* printf("f(2) = "); */
    /* print(fib[2]); fputc('\n', stdout); */

    while (1) {
        add(fib[i-2], fib[i-1], fib[i]);
        /* printf("f(%d) = ", i); */
        /* print(fib[i]); fputc('\n', stdout); */

        fib_digits[i] = digit(fib[i]);

        if (digit(fib[i]) > 101) break;
        i++;
    }
}

int main()
{
    char a[150];
    char b[150];
    int i;
    int l1, l2;
    char *pc;

    // make fib
    make_fib();

    while (1) {
        scanf("%s %s", a, b);

        if (a[0] == '0' && b[0] == '0') 
            break;

        l1 = strlen(a);
        l2 = strlen(b);

        //make long-int number from char arr
        for (i=149, pc = &a[strlen(a) - 1]; pc >= a; i--,pc--) {
            a[i] = *pc - '0';
            /* printf("i = %d, ", i); */
        }
        /* printf("\n"); */
        while (i >= 0) 
            a[i--] = 0;
        for (i=149, pc = &b[strlen(b) - 1]; pc >= b; i--,pc--) {
            b[i] = *pc - '0';
            /* printf("i = %d, ", i); */
        }
        /* printf("\n"); */
        while (i >= 0) 
            b[i--] = 0;

        /* printf("a="); */
        /* print(a); fputc('\n', stdout); */
        /* printf("b="); */
        /* print(b); fputc('\n', stdout); */

        //find same digit fib number
        for (i=0; i<500; i++) {
            if (fib_digits[i] >= l1) {
                /* printf("d(f(%d))=%d\n", i, fib_digits[i]); */
                break;
            }
        }

        //compare to find first fib number larger than or equal to `a`
        for (; i<500; i++) {
            if (compare(fib[i], a) >= 0) {
                l1 = i;
                break;
            }
        }

        for (i=0; i<500; i++) {
            if (fib_digits[i] >= l2) {
                /* printf("d(f(%d))=%d\n", i, fib_digits[i]); */
                break;
            }
        }

        //compare to find first fib number larger than `b`
        for (; i<500; i++) {
            if (compare(fib[i], b) > 0) {
                l2 = i;
                break;
            }
        }

        /* printf("l1=%d, l2=%d\n", l1, l2); */

        printf("%d\n", l2-l1);
    }

    return 0;
}

/* vim: set ts=4 sw=4 : */
