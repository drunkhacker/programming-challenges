#include <stdio.h>
#include <string.h>
#include <map>
#include <stdlib.h>

/* pseudo code
 * n(k, d) = 깊이 d인 complete k-ary 트리의 노드 갯수
 *
 * a(k, 0) = 1
 * a(k, 1) = k!
 * a(k, d) = (n(k, d)-1)!/((n(k, d-1)!)^k) * a(k, d-1)^k
 *           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *           P(k, d) = n(k, d)-1개의 원소를 k등분해서 나누는 방법의 수
 */

#define MAXDIGITS 11000

#define PLUS		1		/* positive sign bit */
#define MINUS		-1		/* negative sign bit */

typedef struct {
    char digits[MAXDIGITS];         /* represent the number */
    int signbit;			/* 1 if positive, -1 if negative */ 
    int lastdigit;			/* index of high-order digit */
} bignum;

void print_bignum(bignum *n);
void int_to_bignum(int s, bignum *n);
void initialize_bignum(bignum *n);
int max(int a, int b);
void add_bignum(bignum *a, bignum *b, bignum *c);
void subtract_bignum(bignum *a, bignum *b, bignum *c);
int compare_bignum(bignum *a, bignum *b);
void zero_justify(bignum *n);
void digit_shift(bignum *n, int d);
void multiply_bignum(bignum *a, bignum *b, bignum *c);
void divide_bignum(bignum *a, bignum *b, bignum *c);

using namespace std;
typedef map< pair<int, int>, bignum > bmap;
bmap ncr;
bmap ans; //ans[k,d] = complete k-ary 트리에서 depth d일때 labeling 방법
long nodes[22][22] = {0,}; //nodes[k][d] = depth d complate k-ary 트리의 총 노드 개수
bignum f[3281];

bignum* nCr(int n, int r)
{
    bignum *a, *b, c;
    bmap::iterator it;
    pair<int, int> key = make_pair(n, r);

    //printf("n=%d, r=%d\n", n, r);

    if (r == 0 || n == r) {
        it = ncr.find(key);
        if (it == ncr.end()) {
            int_to_bignum(1, &c);
            ncr[key] = c;
        }
        return &ncr[key];
    }

    if (r == 1) {
        int_to_bignum(n, &c);
        ncr[key] = c;
        return &ncr[key];
    }

    it = ncr.find(key);
    if (it == ncr.end()) {
        bignum *r_, *n_, *n_r, t;
        //nCr = n! / (n-r)! / r!
        n_ = &f[n];
        n_r = &f[n-r];
        r_ = &f[r];

        divide_bignum(n_, n_r, &t);
        divide_bignum(&t, r_, &c);
        ncr[key] = c;
    }
    return &ncr[key];
}

void partition(int k, int d, bignum *res)
{
    //calculate P(k, d)
    int n, i;
    bignum t, a;
    //printf("partition k=%d, d=%d\n", k, d);

    n = nodes[k][d] - 1;
    int_to_bignum(1, res);

    a = f[n];

    for (i=0; i<k; i++) {
        divide_bignum(&a, &f[nodes[k][d-1]], &t);
        a = t;
    }
    *res = a;
}

void bigpow(bignum *a, int k, bignum *res)
{
    bignum s, t;

    int_to_bignum(1, &s);
    int_to_bignum(1, &t);
    while (k--) {
        //printf("a=");
        //print_bignum(a);
        //printf("s=");
        //print_bignum(&s);
        multiply_bignum(a, &s, &t);
        //printf("t=");
        //print_bignum(&t);
        s = t;
    }

    memcpy(res, &t, sizeof(bignum));
    //printf("bigpow end\n");
}

bignum* compute(int k, int d)
{
    pair<int, int> key = make_pair(k, d);
    bmap::iterator it;
    bignum *prev_ans;
    bignum t;

    it = ans.find(key);

    //printf("k=%d, d=%d\n", k, d);
    if (it == ans.end()) {
        if (d == 0) {
            int_to_bignum(1, &t);
        } else if (d == 1) {
            t = f[k];
        } else {
            bignum t2, t3;

            bigpow(compute(k, d-1), k, &t2);
            partition(k, d, &t3);
            //printf("t2 = "); print_bignum(&t2);
            //printf("t3 = "); print_bignum(&t3);
            multiply_bignum(&t2, &t3, &t);
            //printf("t2*t3=");print_bignum(&t);
        }

        ans[key] = t;
    }

    return &ans[key];
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

long mpow(int x, int y)
{
    long r = 1;
    while (y--)
        r *= x;

    return r;
}

int main()
{
    int i, k, d;
    bignum t, t2, t3;

    for (k=1; k<22; k++) {
        nodes[k][0] = 1;
        for (d=1; d<=21/k; d++) {
            nodes[k][d] = nodes[k][d-1] + mpow(k, d);
        }
    }

    int_to_bignum(1, &f[0]);
    for (i=1; i<=3280; i++) {
        int_to_bignum(i, &t);
        multiply_bignum(&f[i-1], &t, &f[i]);
    }

    for (k=1; k<22; k++) {
        for (d=0; d<=21/k; d++) {
            compute(k, d);
        }
    }

    while(scanf("%d %d", &k, &d) != EOF) {
        //printf("k=%d, d=%d\n", k, d);
        print_bignum(compute(k, d));
    }

    return 0;
}

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
