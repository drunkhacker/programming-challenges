#include <stdio.h>
#include <string.h>

char *f[257][144]; // f[n][k] = 길이 n짜리 조각 중 k번째
char n[257] = {0,};
char file[256] = {0,};
int len; //파일 길이

void concat(int l1, int i, int l2, int j, char *buf)
{
    strcpy(buf, f[l1][i]);
    strcat(buf, f[l2][j]);
}

int try(int l1, int l2)
{
    int i, j, k, m;
    int l1_cand, l2_cand;
    char file2[256] = {0,};
    memset(file, 0, sizeof(file));

    for (i=0; i<n[l1]; i++) {
        l1_cand = i;
        for (j=0; j<n[l2]; j++) {
            l2_cand = j;

            //f[l1][l1_cand] | f[l2][l2_cand] 를 file로 만들어보고 검사
            concat(l1, l1_cand, l2, l2_cand, file);

            /* printf("file=%s\n", file); */
            for (k=0; k<n[l1]; k++) {
                if (k == l1_cand) continue;
                for (m=0; m<n[l2]; m++) {
                    if (m == l2_cand) continue;
                    concat(l1, k, l2, m, file2);

                    /* printf("  file2=%s...", file2); */
                    if (strcmp(file, file2)) {
                        concat(l2, m, l1, k, file2);
                        if (strcmp(file, file2)) {
                            /* printf("wrong\n"); */
                            goto retry;
                        }
                    }
                }
            }
retry:
            if (k == n[l1] && m == n[l2]) 
                return 1;
        }
    }

    return 0;
}

void input()
{
    char buf[BUFSIZ];
    int len;

    memset(n, 0, sizeof(n));
    memset(f, 0, sizeof(f));

    /* printf("input\n"); */
    while(1) {
        if (fgets(buf, BUFSIZ-1, stdin) == NULL)
            return;
        buf[strlen(buf)-1] = 0;
        len = strlen(buf);
        /* printf("len = %d\n", len); */
        if (len == 0) break;

        f[len][n[len]++] = strdup(buf);
    }

    return;
}

void process()
{
    int min_len = 256; //가장 작은 파일 조각 길이
    int max_len = 0; //len - min_len
    int i;

    //find min_len 
    for (i=1; i<=256; i++) {
        if (n[i] > 0 && min_len > i) {
            min_len = i;
        }
    }

    //find max_len 
    for (i=1; i<=256; i++) {
        if (n[i] > 0 && max_len < i) {
            max_len = i;
        }
    }

    len = min_len + max_len;


    //가장 작은조각 + 가장 큰 조각
    if (try(min_len, max_len))
        return;

    //가장 큰조각 + 가장 작은 조각
    try(max_len, min_len);
}

void output()
{
    printf("%s\n", file);
}

int main()
{
    int n, i;
    char buf[BUFSIZ];

    scanf("%d", &n);
    fgets(buf, BUFSIZ-1, stdin);
    fgets(buf, BUFSIZ-1, stdin);

    while (n--) {
        input();
        process();
        output();
        if (n > 0) {
            printf("\n");
        }
    }

    return 0;

}
