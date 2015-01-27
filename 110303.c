#include <stdio.h>
#include <string.h>

#define MIN(a,b) ((a)>(b)?(b):(a))

int cnt[2][26];

int input()
{
    char buf[1002];
    int i, j;
    char *pc;

    memset(cnt, 0, 2*26*sizeof(int));

    for (i=0; i<2; i++) {
        if (fgets(buf, BUFSIZ, stdin) == NULL) return 0;
        pc = buf;
        while (*pc != 0 && *pc != '\n') {
            cnt[i][*pc - 'a']++;
            pc++;
        }
    }
    return 1;
}

void process()
{
    int i;

    for (i=0; i<26; i++) {
        cnt[0][i] = MIN(cnt[0][i], cnt[1][i]);
    }
}

void output()
{
    int i;
    for (i=0; i<26; i++) {
        while (cnt[0][i]--) {
            fputc('a' + i, stdout);
        }
    }
    fputc('\n', stdout);
}

int main()
{
    while(1) {
        if (!input())
            break;
        process();
        output();
    }
    return 0;
}
