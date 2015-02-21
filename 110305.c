#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_N 13000

int cases = 1;

char answer[MAX_N];
char submit[MAX_N];

int input()
{
    int n, i;
    char *pc;

    memset(answer, 0, sizeof(answer));
    memset(submit, 0, sizeof(submit));

    scanf("%d", &n);
    fgets(answer, MAX_N - 1, stdin);

    if (n == 0)
        return 0;

    pc = answer;
    for (i=0; i<n; i++) {
        fgets(pc, MAX_N - 1, stdin);
        pc += strlen(pc);
    }

    scanf("%d", &n);
    fgets(submit, MAX_N - 1, stdin);

    pc = submit;
    for (i=0; i<n; i++) {
        fgets(pc, MAX_N - 1, stdin);
        pc += strlen(pc);
    }

    //debug output
    /*printf("answer\n");*/
    /*printf("%s\n", answer);*/
    /*printf("submit\n");*/
    /*printf("%s\n", submit);*/

    return 1;
}

int pe(char *s1, char *s2)
{
    while (*s1 && *s2) {
        while (*s1 && !isdigit(*s1)) s1++;
        while (*s2 && !isdigit(*s2)) s2++;

        if (*s1 && *s2) {
            if (*s1 != *s2)
                return 0;
        } else {
            while (*s1)
                if (isdigit(*s1++))
                    return 0;

            while (*s2)
                if (isdigit(*s2++))
                    return 0;
        }

        if (*s1) s1++;
        if (*s2) s2++;
    }

    return 1;
}

int process()
{
    //exact match
    if (strcmp(answer, submit) == 0)
        return 0;

    if (pe(answer, submit))
        return 1;

    return 2;
}

void output(int r)
{
    printf("Run #%d: %s\n", cases, r == 0 ? "Accepted" : r == 1 ? "Presentation Error" : "Wrong Answer");
}

int main()
{
    while (input()) {
        output(process());
        cases++;
    }
    return 0;
}
