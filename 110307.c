#include <stdio.h>
#include <string.h>

char *dict[17][26000];
int dict_count[17];

char str1[16];
char str2[16];

int doublet(char *str1, char *str2)
{
    int c = 0;
    int i;

    if (strlen(str1) != strlen(str2)) return 0;

    for (i=0; i<strlen(str1); i++) {
        c += str1[i] != str2[i];
        if (c > 1) return 0;
    }

    return c == 1;
}

void input_dict()
{
    char buf[BUFSIZ];
    int len;

    memset(dict, 0, sizeof(dict));
    memset(dict_count, 0, sizeof(dict_count));

    while (1) {
        fgets(buf, BUFSIZ-1, stdin);
        if (buf[0] == '\n')
            break;
        buf[strlen(buf)-1] = 0;
        len = strlen(buf);

        dict[len][dict_count[len]++] = strdup(buf);
    }
}

void process()
{
    int index1, index2;
    int i;
    int seq_len;

    //find str1, str2 pos
    for (i=0; i<dict_size; i++) {
        if (strcmp(dict[i], str1) == 0)
            index1 = i;
        if (strcmp(dict[i], str2) == 0)
            index2 = i;
    }

    for (seq_len=0; seq_len<dict_size; seq_len++) {
    }

}



int main()
{

    input_dict();
    while(1) {
        if (scanf("%s %s", &str1, &str2) == EOF)
            break;
        process();
        output();
    }

    return 0;
}
