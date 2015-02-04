#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cases;
int num_lines;
char lines[82][100];

char key_sentence[] = "the quick brown fox jumps over the lazy dog";
#define KEY_SENTENCE_LEN 43
char key_cnt[27] = {0,};
char key_cnt_sorted[27] = {0,};
char cnt[27] = {0,};

char map[26] = {0,};

int compare(const void *e1, const void *e2)
{
    return *(char *)e1 - *(char *)e2;
}

void input()
{
    num_lines = 0;

    while(1) {
        if (fgets(lines[num_lines], 81, stdin) == NULL)
            break;
        if (lines[num_lines][0] == '\n')
            break;
        lines[num_lines][strlen(lines[num_lines]) - 1] = 0;
        num_lines++;
    }
}

void make_dist(char *sentence, char *dist)
{
    char *pc = sentence;

    memset(dist, 0, 27);

    while (*pc) {
        if (*pc == ' ') {
            dist[26]++;
        } else {
            dist[*pc - 'a']++;
        }
        pc++;
    }
}

void print_dist(char *dist)
{
    int i;
    for (i=0; i<26; i++) {
        printf("%c: %d ", i + 'a', dist[i]);
    }
    printf("\n");
}

int same_dist(char *sentence)
{
    int i;

    make_dist(sentence, cnt);
    /*print_dist(cnt);*/
    /*print_dist(key_cnt);*/

    qsort(cnt, 26, 1, compare);
    for (i = 0; i < 26; i++)
        if (key_cnt_sorted[i] != cnt[i]) return 0;

    return 1;
}

int same_space_cnt(char *s1, char *s2)
{
    int i, l;
    l = strlen(s1);
    for (i=0; i<l; i++) {
        if (!((s1[i] != ' ' && s2[i] != ' ') || (s1[i] == ' ' && s2[i] == ' ')))
            return 0;
    }

    return 1;
}

int process()
{
    int i;
    char *pc;
    int len;
    char *sentence;
    /*printf("process %d lines\n", num_lines);*/

    /*for (i=0; i<num_lines; i++) {*/
        /*printf("%s\n", lines[i]);*/
    /*}*/

    //find target sentence
    for (i = 0; i < num_lines; i++) {
        len = strlen(lines[i]);
        if (len == KEY_SENTENCE_LEN && same_space_cnt(lines[i], key_sentence) &&
            same_dist(lines[i])) {
            break;
        }
    }

    /*printf("i = %d\n", i);*/

    if (i == num_lines)
        return 0;

    //make map
    memset(map, 0, 26);
    sentence = lines[i];
    for (i = 0; i < KEY_SENTENCE_LEN; i++) {
        if (sentence[i] == ' ') continue;
        if (map[sentence[i] - 'a'] != 0 && map[sentence[i] - 'a'] != key_sentence[i])
            return 0;
        map[sentence[i] - 'a'] = key_sentence[i];
    }

    //decrypt
    for (i = 0; i < num_lines; i++) {
        pc = lines[i];
        while (*pc) {
            if (*pc != ' ') {
                *pc = map[*pc - 'a'];
            }
            pc++;
        }
    }

    return 1;
}

void output()
{
    int i;
    for (i = 0; i < num_lines; i++) {
        printf("%s\n", lines[i]);
    }
    printf("\n");
}

int main()
{
    int i;
    char *pc;

    scanf("%d\n", &cases);
    /*printf("%d cases\n", cases);*/

    make_dist(key_sentence, key_cnt);
    make_dist(key_sentence, key_cnt_sorted);
    qsort(key_cnt_sorted, 26, 1, compare);

    for (i=0; i<cases; i++) {
        input();
        if (process()) 
            output();
        else
            printf("No solution.\n\n");
    }

    return 0;
}
/* vim: set ts=4 sw=4 : */
