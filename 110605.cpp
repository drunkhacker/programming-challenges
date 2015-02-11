#include <stdio.h>
#include <string.h>
#include <map>
#include <stdlib.h>

/* bignum */
#define	MAXDIGITS	1000	/* maximum length bignum */ 

#define PLUS		1		/* positive sign bit */
#define MINUS		-1		/* negative sign bit */

typedef struct {
    char digits[MAXDIGITS];         /* represent the number */
    int signbit;			/* 1 if positive, -1 if negative */ 
    int lastdigit;			/* index of high-order digit */
} bignum;


void int_to_bignum(int s, bignum *n);
void add_bignum(bignum *a, bignum *b, bignum *c);
void subtract_bignum(bignum *a, bignum *b, bignum *c);
int compare_bignum(bignum *a, bignum *b);
void zero_justify(bignum *n);
void print_bignum(bignum *n);
void digit_shift(bignum *n, int d);
void multiply_bignum(bignum *a, bignum *b, bignum *c);
void divide_bignum(bignum *a, bignum *b, bignum *c);

void print_bignum(bignum *n)
{
	int i;

    if (n->signbit == MINUS) printf("- ");
    for (i=n->lastdigit; i>=0; i--)
        printf("%c",'0'+ n->digits[i]);

    printf("\n");
}

void int_to_bignum(int s, bignum *n)
{
    int i;				/* counter */
    int t;				/* int to work with */

    if (s >= 0) n->signbit = PLUS;
    else n->signbit = MINUS;

    for (i=0; i<MAXDIGITS; i++) n->digits[i] = (char) 0;

    n->lastdigit = -1;

    t = abs(s);

    while (t > 0) {
        n->lastdigit ++;
        n->digits[ n->lastdigit ] = (t % 10);
        t = t / 10;
    }

    if (s == 0) n->lastdigit = 0;
}

void initialize_bignum(bignum *n)
{
    int_to_bignum(0,n);
}


int max(int a, int b)
{
    if (a > b) return(a); else return(b);
}

void add_bignum(bignum *a, bignum *b, bignum *c)
{
    int carry;			/* carry digit */
    int i;				/* counter */

    initialize_bignum(c);

    if (a->signbit == b->signbit) c->signbit = a->signbit;
    else {
        if (a->signbit == MINUS) {
            a->signbit = PLUS;
            subtract_bignum(b,a,c);
            a->signbit = MINUS;
        } else {
            b->signbit = PLUS;
            subtract_bignum(a,b,c);
            b->signbit = MINUS;
        }
        return;
    }

    c->lastdigit = max(a->lastdigit,b->lastdigit)+1;
    carry = 0;

    for (i=0; i<=(c->lastdigit); i++) {
        c->digits[i] = (char) (carry+a->digits[i]+b->digits[i]) % 10;
        carry = (carry + a->digits[i] + b->digits[i]) / 10;
    }

    zero_justify(c);
}


void subtract_bignum(bignum *a, bignum *b, bignum *c)
{
    int borrow;			/* has anything been borrowed? */
    int v;				/* placeholder digit */
    int i;				/* counter */

    initialize_bignum(c);

    if ((a->signbit == MINUS) || (b->signbit == MINUS)) {
        b->signbit = -1 * b->signbit;
        add_bignum(a,b,c);
        b->signbit = -1 * b->signbit;
        return;
    }

    if (compare_bignum(a,b) == PLUS) {
        subtract_bignum(b,a,c);
        c->signbit = MINUS;
        return;
    }

    c->lastdigit = max(a->lastdigit,b->lastdigit);
    borrow = 0;

    for (i=0; i<=(c->lastdigit); i++) {
        v = (a->digits[i] - borrow - b->digits[i]);
        if (a->digits[i] > 0)
            borrow = 0;
        if (v < 0) {
            v = v + 10;
            borrow = 1;
        }

        c->digits[i] = (char) v % 10;
    }

    zero_justify(c);
}

int compare_bignum(bignum *a, bignum *b)
{
    int i;				/* counter */

    if ((a->signbit == MINUS) && (b->signbit == PLUS)) return(PLUS);
    if ((a->signbit == PLUS) && (b->signbit == MINUS)) return(MINUS);

    if (b->lastdigit > a->lastdigit) return (PLUS * a->signbit);
    if (a->lastdigit > b->lastdigit) return (MINUS * a->signbit);

    for (i = a->lastdigit; i>=0; i--) {
        if (a->digits[i] > b->digits[i]) return(MINUS * a->signbit);
        if (b->digits[i] > a->digits[i]) return(PLUS * a->signbit);
    }

    return(0);
}

void zero_justify(bignum *n)
{
    while ((n->lastdigit > 0) && (n->digits[ n->lastdigit ] == 0))
        n->lastdigit --;

    if ((n->lastdigit == 0) && (n->digits[0] == 0))
        n->signbit = PLUS;	/* hack to avoid -0 */
}


void digit_shift(bignum *n, int d)		/* multiply n by 10^d */
{
    int i;				/* counter */

    if ((n->lastdigit == 0) && (n->digits[0] == 0)) return;

    for (i=n->lastdigit; i>=0; i--)
        n->digits[i+d] = n->digits[i];

    for (i=0; i<d; i++) n->digits[i] = 0;

    n->lastdigit = n->lastdigit + d;
}



void multiply_bignum(bignum *a, bignum *b, bignum *c)
{
    bignum row;			/* represent shifted row */
    bignum tmp;			/* placeholder bignum */
    int i,j;			/* counters */

    initialize_bignum(c);

    row = *a;

    for (i=0; i<=b->lastdigit; i++) {
        for (j=1; j<=b->digits[i]; j++) {
            add_bignum(c,&row,&tmp);
            *c = tmp;
        }
        digit_shift(&row,1);
    }

    c->signbit = a->signbit * b->signbit;

    zero_justify(c);
}


void divide_bignum(bignum *a, bignum *b, bignum *c)
{
    bignum row;                     /* represent shifted row */
    bignum tmp;                     /* placeholder bignum */
    int asign, bsign;		/* temporary signs */
    int i,j;                        /* counters */

    initialize_bignum(c);

    c->signbit = a->signbit * b->signbit;

    asign = a->signbit;
    bsign = b->signbit;

    a->signbit = PLUS;
    b->signbit = PLUS;

    initialize_bignum(&row);
    initialize_bignum(&tmp);

    c->lastdigit = a->lastdigit;

    for (i=a->lastdigit; i>=0; i--) {
        digit_shift(&row,1);
        row.digits[0] = a->digits[i];
        c->digits[i] = 0;
        while (compare_bignum(&row,b) != PLUS) {
            c->digits[i] ++;
            subtract_bignum(&row,b,&tmp);
            row = tmp;
        }
    }

    zero_justify(c);

    a->signbit = asign;
    b->signbit = bsign;
}

/* bignum end */

using namespace std; 

map< pair<int, int>, bignum > ncr;
int k, d;

bignum a[22] = {0,}; //a[d] = complete k-ary 트리에서 depth d일때 labeling 방법
bignum f[22] = {0,}; //f[n] = n!
int n[21] = {0,}; //n[d] = depth d complate k-ary 트리의 총 노드 개수

bignum *nCr(int n, int r)
{
    bignum *a, *b, c;
    map< pair<int, int>, bignum >::iterator it;

    if (r == 0 || n == r) {
        it = ncr.find(make_pair(n, r));
        if (it == ncr.end()) {
            int_to_bignum(1, &c);
            ncr[make_pair(n, r)] = c;
        }
        return &ncr[make_pair(n, r)];
    }

    it = ncr.find(make_pair(n, r));
    if (it == ncr.end()) {
        a = nCr(n-1, r-1);
        b = nCr(n-1, r);
        add_bignum(a, b, &c);
        ncr[make_pair(n, r)] = c;
    }
    return &ncr[make_pair(n, r)];
}



//    k    d   N
//    2   10   2**11 - 1 = 2047
//    3    7   1 + 3 + 3^2 + ... + 3^7 = 3280
//    4    5   1365
//    5    4   781
//    6    3   259
//    7    3   400
//    8    2   73
//    9    2   91
//    10   2   111
//    11   1   12
//    12   1   13
//    13   1   14
//    ....
//    21   1   21

int mpow(int x, int y)
{
    long r = 1;
    while (y--)
        r *= x;

    return r;
}

bignum *process()
{
    int i,j;
    int nodes;
    int part;
    bignum t;

    n[0] = 1;
    for (i=1; i<=d; i++) {
        n[i] = n[i-1] + mpow(k, i);
    }

    for (i=0; i<22; i++)
        initialize_bignum(&a[i]);

    int_to_bignum(1, &a[0]);
    memcpy(&a[1], &f[k], sizeof(bignum));

    for (i=2; i<=d; i++) {
        nodes = n[i] - 1;
        part = (nodes)/k;

        //printf("nodes = %d, part = %d\n", nodes, part);
        //print_bignum(nCr(nodes, part));

        int_to_bignum(1, &a[i]);
        while (nodes > 0) {
            multiply_bignum(&a[i], nCr(nodes, part), &t);
            //printf("nodes = %d, part = %d\n", nodes, part);
            nodes -= part;
            memcpy(&a[i], &t, sizeof(bignum));
        }
        //printf("hihi\n");
        multiply_bignum(&t, &f[k], &a[i]);
        multiply_bignum(&a[i], &a[i-1], &t);
        memcpy(&a[i], &t, sizeof(bignum));
    }

    return &a[d];
}

int main()
{
    int i;
    bignum t, t2, t3;

    int_to_bignum(12, &t);
    int_to_bignum(12, &t2);
    multiply_bignum(&t, &t2, &t3);
    print_bignum(nCr(7,3));
    print_bignum(nCr(4,3));

    int_to_bignum(1, &f[0]);
    //print_bignum(&f[0]);
    for (i=1; i<22; i++) {
        int_to_bignum(i, &t);
        multiply_bignum(&f[i-1], &t, &f[i]);
        //print_bignum(&f[i]);
    }

    while(scanf("%d %d", &k, &d) != EOF) {
        print_bignum(process());
    }

    return 0;
}
