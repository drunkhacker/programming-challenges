#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define TRIPLE(a,b,c) ((a)*13*13 + (b)*13 + (c))
#define QUADRUPLE(a,b,c,d) ((a)*13*13*13 + (b)*13*13 + (c)*13 + (d))

/* hand rank encoding 
 * [Straight Flush] [Four of a Kind] [Full House] [Flush] [Straight] [Three of a Kind] [Two Pairs] [Pair] [High Card] 
 *        8|x               7|x         6|x       5|xxxxx   4|x             3|x          2|xxx     1|xxxx   0|xxxxx
 * x: denote 1digit base(13) number
 * 13^6 = 4826809 < 10000000. so, category number can be encoded (c)*1e7
 */
 
/* for card encoding */
char *suits = "CDHS";
char *values = "23456789TJQKA";
int value_map[100] = {0,};

int hands[2][5];

int rank_card(char suit, char value) 
{
    /*printf("suit=%c, value=%c\n", suit, value);*/
    /*ex : 2C -> 0, 2D -> 1, 2H -> 2, 2S -> 3, 3C -> 4*/
    int i,j;

    for (i=0; i<4; i++)
        if (suits[i] == suit) break;
    for (j=0; j<13; j++)
        if (values[j] == value) break;

    return j*4 + i;
}

char suit(int card)
{
    return suits[card%4];
}

int value(int card)
{
    return value_map[values[card/4]];
}

int samevalue(int cards[], int len)
{
    int i;
    int v;

    v = value(cards[0]);
    for (i=1; i<len; i++)
        if (value(cards[i]) != v) {
            /*printf("%d != %d, i=%d, len=%d, card=%d\n", v, value(cards[i]), i, len, cards[i]);*/
            return -1;
        }

    return v;
}

int samesuit(int cards[], int len)
{
    int i;
    char s;

    s = suit(cards[0]);
    for (i=1; i<len; i++)
        if (suit(cards[i]) != s) return -1;

    return s;
}

int consecutive_values(int cards[], int len)
{
    int v;
    int i;

    v = value(cards[0]);
    for (i=0; i<len; i++, v--)
        if (value(cards[i]) != v) return -1;

    return value(cards[0]);
}

int card_compare(const void *i, const void *j)
{
    return value(*(int *)j) - value(*(int *)i);
}

int input()
{
    int i, j;
    char s, v;
    char buf[BUFSIZ];
    char *pch;

    if (fgets(buf, BUFSIZ - 1, stdin) != NULL) {
        pch = buf;
        for (i=0; i<2; i++) {
            for (j=0; j<5; j++) {
                v = *pch++;
                s = *pch++;
                hands[i][j] = rank_card(s, v);
                pch++; /* for space or newline */
            }
        }

        /*
        printf("== input end ==\n");
        for (i=0; i<2; i++) {
            for (j=0; j<5; j++) {
                printf("%d%c ", value(hands[i][j]), suit(hands[i][j]));
            }
            printf("\n");
        }
        */
        return 1;
    }

    return 0;
}

int check_straight_flush(int hands[])
{
    int i;
    char s;
    int v;

    /* check suit all same */
    if (samesuit(hands, 5) < 0)
        return 0;

    /* check consecutive value */
    if ((v = consecutive_values(hands, 5)) >= 0)
        return (int)(8*1e7) + v;

    return 0;
}

int check_4kind(int hands[])
{
    int v;

    /* case 1) 0..3 : same, 4: differ */
    if ((v = samevalue(hands, 4)) >= 0)
        return (int)(7*1e7) + v;

    /* case 2) 0: differ, 1..4: same */
    if ((v = samevalue(hands + 1, 4)) >= 0)
        return (int)(7*1e7) + v;

    return 0;
}

int check_fullhouse(int hands[])
{
    int i;
    int v;

    /*printf("check fullhouse case1\n");*/

    /* case 1) x x x p p */
    if ((v = samevalue(hands, 3)) >= 0 && samevalue(hands + 3, 2) >= 0)
        return (int)(6*1e7) + v;

    /*printf("check fullhouse case2\n");*/
    /* case 2) p p x x x */
    if ((v = samevalue(hands + 2, 3)) >= 0 && samevalue(hands, 2) >= 0) 
        return (int)(6*1e7) + v;

    /*printf("no\n");*/
    return 0;
}

int highcard(int hands[])
{
    int i;
    int v = 0;

    for (i=0; i<5; i++) {
        v = v*13 + value(hands[i]);
    }

    return v;
}

int check_flush(int hands[])
{
    /* all same suit */
    if (samesuit(hands, 5) < 0)
        return 0;
    return (int)(5*1e7) + highcard(hands);
}

int check_straight(int hands[])
{
    int v;
    if ((v = consecutive_values(hands, 5)) >= 0)
        return (int)(4*1e7) + v;
    return 0;
}

int check_3kind(int hands[])
{
    int v;
    /* case 1) x y s s s */
    if ((v = samevalue(hands + 2, 3)) >= 0)
        return (int)(3*1e7) + v;
    /* case 2) x s s s y */
    if ((v = samevalue(hands + 1, 3)) >= 0)
        return (int)(3*1e7) + v;
    /* case 3) s s s x y */
    if ((v = samevalue(hands, 3)) >= 0)
        return (int)(3*1e7) + v;

    return 0;
}

int check_2pair(int hands[])
{
    int v1, v2;
    /* case 1) x x y y s */
    if ((v1 = samevalue(hands, 2)) >= 0 && (v2 = samevalue(hands + 2, 2)) >= 0)
        return (int)(2*1e7) + TRIPLE(MAX(v1, v2), MIN(v1, v2), value(hands[4]));

    /* case 2) x x s y y */
    if ((v1 = samevalue(hands, 2)) >= 0 && (v2 = samevalue(hands + 3, 2)) >= 0)
        return (int)(2*1e7) + TRIPLE(MAX(v1, v2), MIN(v1, v2), value(hands[2]));

    /* case 3) s x x y y */
    if ((v1 = samevalue(hands + 1, 2)) >= 0 && (v2 = samevalue(hands + 3, 2)) >= 0)
        return (int)(2*1e7) + TRIPLE(MAX(v1, v2), MIN(v1, v2), value(hands[0]));

    return 0;
}

int check_1pair(int hands[])
{
    int v;
    /* case 1) a a x y z */
    if ((v = samevalue(hands, 2)) >= 0)
        return (int)(1*1e7) + QUADRUPLE(v, value(hands[2]), value(hands[3]), value(hands[4]));
    /* case 2) x a a y z */
    if ((v = samevalue(hands + 1, 2)) >= 0)
        return (int)(1*1e7) + QUADRUPLE(v, value(hands[0]), value(hands[3]), value(hands[4]));
    /* case 3) x y a a z */
    if ((v = samevalue(hands + 2, 2)) >= 0)
        return (int)(1*1e7) + QUADRUPLE(v, value(hands[0]), value(hands[1]), value(hands[4]));
    /* case 4) x y z a a */
    if ((v = samevalue(hands + 3, 2)) >= 0)
        return (int)(1*1e7) + QUADRUPLE(v, value(hands[0]), value(hands[1]), value(hands[2]));

    return 0;
}

int (*rank_func[9])(int hands[]) = {check_straight_flush, check_4kind, check_fullhouse, 
    check_flush, check_straight, check_3kind, check_2pair, check_1pair, highcard};

int process()
{
    int i,j;
    int r1, r2;

    qsort(hands[0], 5, sizeof(int), card_compare);
    qsort(hands[1], 5, sizeof(int), card_compare);

    /*
    for (i=0; i<2; i++) {
        for (j=0; j<5; j++) {
            printf("%d%c(=%d) ", value(hands[i][j]), suit(hands[i][j]), hands[i][j]);
        }
        printf("\n");
    }
    */

    /* hand 1 */
    for (i=0; i<9; i++)
        if ((r1 = rank_func[i](hands[0])) > 0) break;
    /*printf("rank_func[%d]\n", i);*/

    /* hand 2 */
    for (i=0; i<9; i++)
        if ((r2 = rank_func[i](hands[1])) > 0) break;
    /*printf("rank_func[%d]\n", i);*/

    /*printf("r1 = %d, r2 = %d\n", r1, r2);*/
    return r1 - r2;
}

int main()
{
    int i;
    int r;
    /*initialize value map (char => int)*/
    for (i=0; i<13; i++) {
        char v = values[i];
        value_map[(int)v] = i;
    }

    while (1) {
        if (!input())
            break;
        r = process();
        if (r != 0)
            printf("%s wins.\n", r > 0 ? "Black" : "White");
        else
            printf("Tie.\n");
    }
    return 0;
}

/* vim: set ts=4 sw=4 : */
