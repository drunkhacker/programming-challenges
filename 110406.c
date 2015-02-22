#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000
#define MAX_LEN 50

int tolls[24];
char records[MAX_N][MAX_LEN];
int record_len;

int cmpr_license(const void *s1, const void *s2)
{
    char *pc1, *pc2;
    int r;

    pc1 = strchr((char *)s1, ' ');
    pc2 = strchr((char *)s2, ' ');

    *pc1 = 0; *pc2 = 0;

    r = strcmp((char *)s1, (char *)s2);
    *pc1 = ' '; *pc2 = ' ';

    return r;
}

int cmpr_date(const void *s1, const void *s2)
{
    char *pc1, *pc2;
    int r;

    pc1 = strchr((char *)s1, ' ');
    pc2 = strchr((char *)s2, ' ');

    return strncmp(pc1+1, pc2+1, 11);
}

void input()
{
    int i;
    for (i=0; i<24; i++) {
        scanf("%d", &tolls[i]);
    }
    fgets(records[0], MAX_LEN-1, stdin);

    i = 0;
    while (1) {
        if (fgets(records[i], MAX_LEN-1, stdin) == NULL)
            break;
        if (records[i][0] == '\n')
            break;
        records[i][strlen(records[i]) - 1] = 0;
        i++;
    }

    record_len = i;
}

void process_license(char *license, int from, int to)
{
    int i;
    char *record;
    char *save;
    char *tok;
    int k;
    int enter_hour, exit_hour, hour;
    int entered = 0;
    int pos_start, pos_end, pos;
    int paired = 0;
    int total_payment = 0;
    int cnt = 0;

    /*printf("=== process license ===\n");*/
    /*printf("from = %d, to = %d\n", from, to);*/

    qsort((void *)records + MAX_LEN*from, to - from, MAX_LEN, cmpr_date);
    for (i=from; i<to; i++) {
        record = records[i];

        k = 0;
        for (tok = strtok_r(record, ": ", &save); tok; tok = strtok_r(NULL, ": ", &save), k++) {
            switch (k) {
                case 0: //license
                case 1: //month
                case 2: //day
                    break;
                case 3: //hour
                    hour = atoi(tok);
                    break;
                case 4: //minute
                    break;
                case 5:
                    if (strcmp(tok, "enter") == 0) {
                        entered = 1;
                    } else if (strcmp(tok, "exit") == 0) {
                        if (entered) {
                            paired = 1;
                        }
                        entered = 0;
                    }
                    break;
                case 6: //km
                    pos = atoi(tok);
                    break;
            }
        }

        if (entered) {
            enter_hour = hour;
            pos_start = pos;
        } else {
            exit_hour = hour;
            pos_end = pos;
        }

        if (paired) {
            /*printf("toll=%d, enter_hour=%d, distance=%d\n", tolls[enter_hour], enter_hour, abs(pos_end - pos_start));*/
            total_payment += tolls[enter_hour]*abs(pos_end-pos_start) + 100;
            paired = 0;
            cnt++;
        }
    }

    if (cnt > 0)  {
        total_payment += 200;
        printf("%s $%d.%02d\n", license, total_payment/100, total_payment%100);
    }
}

void process()
{
    int i = 0;
    int old_i = 0;
    char license[21] = {0,};
    char *pc, *pc2;
    int n;

    //sort by license first
    qsort(records, record_len, MAX_LEN, cmpr_license);

    pc = strchr(records[0], ' ');
    *pc = 0;
    strcpy(license, records[0]);
    *pc = ' ';

    while (i < record_len) {
        //check license has changed
        pc = strchr(records[i], ' ');
        *pc = 0;
        if (strcmp(license, records[i]) != 0) {
            //process old license
            /*printf("old license=%s", license);*/
            process_license(license, old_i, i);

            //new license from now
            strcpy(license, records[i]);
            old_i = i;
        }
        *pc = ' ';
        i++;
    }
    /*printf("old license=%s", license);*/
    process_license(license, old_i, i);
}

int main()
{
    int cases, i, j;
    int first = 1;

    scanf("%d", &cases);
    fgets(records[0], MAX_LEN-1, stdin);

    for (i=0; i<cases; i++) {
        input();
        if (first) {
            first = 0;
        } else {
            printf("\n");
        }
        process();
    }

    return 0;
}

