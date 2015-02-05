#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sched_cnt;
int sched_length[110] = {0,};
int sched_start[110] = {0,};

int nap_cnt = 0;
int nap_length[110] = {0,};
int nap_start[110] = {0,};

int max_nap_index;
int day;

int input()
{
    int i;
    char buf[BUFSIZ];
    char *token;
    char *pc;
    char *tmp;

    int st, fn, t;

    nap_cnt = 0;
    memset(nap_length, 0, sizeof(nap_length));
    memset(nap_start, 0, sizeof(nap_start));

    if (scanf("%d", &sched_cnt) == EOF)
        return 0;

    fgetc(stdin);

    for (i=0; i<sched_cnt; i++) {
        if (fgets(buf, BUFSIZ-1, stdin) == NULL)
            return 0;

        st = fn = 0;
        t = 0;

        //parse time
        pc = buf;
        while (1) {
            token = strtok_r(pc, " :", &tmp);
            if (token == NULL)
                break;
            if (t > 1) 
                break;

            /* printf("token = %s\n", token); */
            if (t == 0) {
                if (st == 0)
                    st = atoi(token)*60;
                else {
                    st += atoi(token);
                    t = 1;
                }
            } else {
                if (fn == 0)
                    fn = atoi(token)*60;
                else {
                    fn += atoi(token);
                    t = 2;
                }
            }

            pc = NULL;
        }

        sched_start[i] = st;
        sched_length[i] = fn - st;
    }

    return 1;
}

void process()
{
    int st, fn;
    int prev_st, prev_fn;

    int i, j, t;

    //sort schedule
    for (i=0; i<sched_cnt-1; i++) {
        for (j=i+1; j<sched_cnt; j++) {
            if (sched_start[i] > sched_start[j]) {
                t = sched_start[i];
                sched_start[i] = sched_start[j];
                sched_start[j] = t;

                t = sched_length[i];
                sched_length[i] = sched_length[j];
                sched_length[j] = t;
            }
        }
    }

    // populate nap time
    prev_st = 0;
    prev_fn = 10*60;
    for (i=0; i<sched_cnt; i++) {
        st = sched_start[i];
        fn = sched_length[i] + st;

        nap_start[nap_cnt] = prev_fn;
        nap_length[nap_cnt++] = st - prev_fn;

        prev_st = st;
        prev_fn = fn;
    }
    nap_start[nap_cnt] = prev_fn;
    nap_length[nap_cnt++] = 18*60 - prev_fn;

    //find longest nap
    max_nap_index = 0;

    for (i=nap_cnt-1; i>=0; i--) {
        if (nap_length[max_nap_index] <= nap_length[i]) {
            max_nap_index = i; 
        }
    }
}

void output()
{
    int st = nap_start[max_nap_index];
    int d = nap_length[max_nap_index];

    printf("Day #%d: the longest nap starts at %02d:%02d and will last for ",
            day, st/60, st%60);

    if (d >= 60)
        printf("%d hours and ", d/60);
    printf("%d minutes.\n", d%60);
}

int main()
{
    day = 1;

    while (1) {
        if (!input())
            break;
        process();
        output();
        day++;
    }
    return 0;
}
