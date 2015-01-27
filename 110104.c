#include <stdio.h>
#include <string.h>

#define MAX_S 10
#define MAX_N 99999999
#define MAX_DIGIT 8

int s, n, d;
char buf[10];
char lines[(2*MAX_S + 3)][(MAX_S + 2)*MAX_DIGIT + (MAX_DIGIT - 1)];

int input()
{
    memset(lines, 0, sizeof(lines));
    scanf("%d %s", &s, buf);

    if (s == 0 && buf[0] == '0')
        return 0;

    return 1;
}

void draw(char num, int l, int r)
{
    char a,b,c,d,e,f,g;
    int i;
    /* 7 segment 처럼 그린다 */
    /*   a
     * l--- r
     * |   |
     f |   | b
     * | g |
     *  ---
     * |   |
     e |   | c
     * |   |
     *  ---
     *   d
     */

    switch (num) {
        case 0: a = b = c = d = e = f = 1; g = 0; break;
        case 1: b = c = 1; a = d = e = f = g = 0; break;
        case 2: a = b = g = e = d = 1; f = c = 0; break;
        case 3: a = b = c = d = g = 1; f = e = 0; break;
        case 4: b = c = g = f = 1; a = e = d = 0; break;
        case 5: a = f = g = c = d = 1; b = e = 0; break;
        case 6: a = f = e = d = c = g = 1; b = 0; break;
        case 7: a = b = c = 1; d = e = f = g = 0; break;
        case 8: a = b = c = d = e = f = g = 1; break;
        case 9: a = b = c = d = f = g = 1; e = 0; break;
    }

    if (a) {
        for (i = l + 1; i < r - 1; i++) 
            lines[0][i] = '-';
    }

    if (b) {
        for (i = 1; i < s + 1; i++)
            lines[i][r - 1] = '|';
    }

    if (c) {
        for (i = s + 2; i < 2*s + 2; i++)
            lines[i][r - 1] = '|';
    }

    if (d) {
        for (i = l + 1; i < r - 1; i++) 
            lines[2*s + 3 - 1][i] = '-';
    }

    if (e) {
        for (i = s + 2; i < 2*s + 2; i++)
            lines[i][l] = '|';
    }

    if (f) {
        for (i = 1; i < s + 1; i++)
            lines[i][l] = '|';
    }

    if (g) {
        for (i = l + 1; i < r - 1; i++) 
            lines[s + 1][i] = '-';
    }
}

void process()
{
    int tmp = n;
    int i;
    char digits[MAX_DIGIT];
    char c;
    int l,r,t,b;
    char *cp;
    d = strlen(buf);

    cp = buf;
    for (cp=buf, i=0; *cp; i++) {
        digits[i] = *cp++ - '0';
    }

    for (i = 0; i < d; i++) {
        c = digits[i];
        l = i*(s + 2) + i;
        r = l + s + 2; /* [l, r) */
        t = 0;
        b = 2*s + 3; /* [t, b) */
        draw(c, l, r);
    }
}

void output()
{
    int i, j;
    char c;
    for (i = 0; i < 2*s + 3; i++) {
        for (j = 0; j < (s + 2)*d + (d-1); j++) {
            c = lines[i][j] == 0 ? ' ' : lines[i][j];
            fputc(c, stdout);
        }
        fputc('\n', stdout);
    }
    fputc('\n', stdout);
}

int main()
{
    while (input()) {
        process();
        output();
    }

    return 0;
}


/* vim: set ts=4 sw=4 : */
