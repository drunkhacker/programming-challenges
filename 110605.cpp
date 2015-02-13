#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* pseudo code
 * n(k, d) = 깊이 d인 complete k-ary 트리의 노드 갯수
 *
 * a(k, 0) = 1
 * a(k, 1) = k!
 * a(k, d) = (n(k, d)-1)!/((n(k, d-1)!)^k) * a(k, d-1)^k
 *
 *                        /   a(k, d-1) \ K
 *         = (n(k, d)-1)! | ------------ |
 *                        \  n(k, d-1)! /
 *
 * ==>    a(k, d)       /  a(k, d-1) \ K
 *     ------------  = | ------------ |
 *      (n(k,d)-1)!     \ n(k, d-1)! /
 *
 * ==>    a(k, d)       /  a(k, d-1) \ K      1
 *      -----------  = | ------------ |  x -------
 *        n(k,d)!       \ n(k, d-1)! /      n(k,d)
 *
 * Let, 
 *            n(k,d)!
 * c(k,d) =  --------- , then
 *            a(k,d)
 *
 * c(k,d) = n(d)*(c(k,d-1)^k)
 */

#define MAXDIGITS 11000

#define PLUS		1		/* positive sign bit */
#define MINUS		-1		/* negative sign bit */

typedef struct bignum {
    char digits[MAXDIGITS];         /* represent the number */
    int signbit;			/* 1 if positive, -1 if negative */ 
    int lastdigit;			/* index of high-order digit */

    bignum () {memset(digits, 0, sizeof(digits));}
    bignum (int number) { *this = number; }
    bignum (const char *number) { *this = number; }

    bignum operator = (char *num);
    bignum operator = (int num);

    bool operator <  (const bignum& b) const;
    bool operator >  (const bignum& b) const { return b < *this; }
    bool operator <= (const bignum& b) const { return !(b < *this); }
    bool operator >= (const bignum& b) const { return !(*this < b); }
    bool operator != (const bignum& b) const { return b < *this || *this < b; }
    bool operator == (const bignum& b) const { return !(b != *this); }

    bignum operator + (bignum& b);
    bignum operator * (bignum& b);
    bignum operator - (bignum& b);
    bignum operator / (bignum& b);
    bignum operator ^ (int k);

    void put();

    void zero_justify();
    void digit_shift(int d);
} bignum;


long n[22][22] = {0,}; //n[k][d] = depth d complate k-ary 트리의 총 노드 개수
bignum f[3281];
bignum c[22][22];


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
    bignum t;

    for (k=1; k<22; k++) {
        n[k][0] = 1;
        for (d=1; d<=21/k; d++) {
            n[k][d] = n[k][d-1] + mpow(k, d);
        }
    }

    f[0] = 1;
    for (i=1; i<=3280; i++) {
        t = i;
        f[i] = f[i-1] *t;
    }

    for (k=1; k<22; k++) {
        c[k][0] = 1;
        for (d=1; d<=21/k; d++) {
            t = n[k][d];
            c[k][d] = (c[k][d-1]^k)*t;
        }
    }

    while(scanf("%d %d", &k, &d) != EOF) {
        (f[n[k][d]]/c[k][d]).put();
    }

    return 0;
}

void bignum::put()
{
    int i;

    if (signbit == MINUS) printf("- ");
    for (i=lastdigit; i>=0; i--)
        printf("%c",'0'+ digits[i]);

    printf("\n");
}

bignum bignum::operator = (int s)
{
    int i;				/* counter */
    int t;				/* int to work with */

    if (s >= 0) signbit = PLUS;
    else signbit = MINUS;

    for (i=0; i<MAXDIGITS; i++) digits[i] = (char) 0;

    lastdigit = -1;

    t = abs(s);

    while (t > 0) {
        lastdigit ++;
        digits[ this->lastdigit ] = (t % 10);
        t = t / 10;
    }

    if (s == 0) lastdigit = 0;

    return *this;
}

int max(int a, int b)
{
    if (a > b) return(a); else return(b);
}

bignum bignum::operator + (bignum &b)
{
    int carry;			/* carry digit */
    int i;				/* counter */

    bignum c = 0;

    if (this->signbit == b.signbit) c.signbit = this->signbit;
    else {
        if (this->signbit == MINUS) {
            this->signbit = PLUS;
            c = b-*this;
            this->signbit = MINUS;
        } else {
            b.signbit = PLUS;
            c = *this-b;
            b.signbit = MINUS;
        }
        return c;
    }

    c.lastdigit = max(this->lastdigit,b.lastdigit)+1;
    carry = 0;

    for (i=0; i<=(c.lastdigit); i++) {
        c.digits[i] = (char) (carry+this->digits[i]+b.digits[i]) % 10;
        carry = (carry + this->digits[i] + b.digits[i]) / 10;
    }

    c.zero_justify();

    return c;
}


bignum bignum::operator - (bignum &b)
{
    int borrow;			/* has anything been borrowed? */
    int v;				/* placeholder digit */
    int i;				/* counter */

    bignum c = 0;

    if ((this->signbit == MINUS) || (b.signbit == MINUS)) {
        b.signbit = -1 * b.signbit;
        c = *this + b;
        b.signbit = -1 * b.signbit;
        return c;
    }

    if (*this > b) {
        c = b - *this;
        c.signbit = MINUS;
        return c;
    }

    c.lastdigit = max(this->lastdigit,b.lastdigit);
    borrow = 0;

    for (i=0; i<=(c.lastdigit); i++) {
        v = (this->digits[i] - borrow - b.digits[i]);
        if (this->digits[i] > 0)
            borrow = 0;
        if (v < 0) {
            v = v + 10;
            borrow = 1;
        }

        c.digits[i] = (char) v % 10;
    }

    c.zero_justify();

    return c;
}

bool bignum::operator < (const bignum &b) const
{
    int i;				/* counter */

    if ((this->signbit == MINUS) && (b.signbit == PLUS)) return false;
    if ((this->signbit == PLUS) && (b.signbit == MINUS)) return true;

    if (b.lastdigit > this->lastdigit) return (PLUS * this->signbit) < 0;
    if (this->lastdigit > b.lastdigit) return (MINUS * this->signbit) < 0;

    for (i = this->lastdigit; i>=0; i--) {
        if (this->digits[i] > b.digits[i]) return(MINUS * this->signbit) < 0;
        if (b.digits[i] > this->digits[i]) return(PLUS * this->signbit) < 0;
    }

    return false;
}

void bignum::zero_justify()
{
    while ((this->lastdigit > 0) && (this->digits[ this->lastdigit ] == 0))
        this->lastdigit --;

    if ((this->lastdigit == 0) && (this->digits[0] == 0))
        this->signbit = PLUS;	/* hack to avoid -0 */
}


void bignum::digit_shift(int d)		/* multiply n by 10^d */
{
    int i;				/* counter */

    if ((this->lastdigit == 0) && (this->digits[0] == 0)) return;

    for (i=this->lastdigit; i>=0; i--)
        this->digits[i+d] = this->digits[i];

    for (i=0; i<d; i++) this->digits[i] = 0;

    this->lastdigit = this->lastdigit + d;
}



bignum bignum::operator * (bignum &b)
{
    bignum row;			/* represent shifted row */
    bignum tmp;			/* placeholder bignum */
    int i,j;			/* counters */

    bignum c = 0;

    row = *this;

    for (i=0; i<=b.lastdigit; i++) {
        for (j=1; j<=b.digits[i]; j++) {
            tmp = c + row;
            c = tmp;
        }
        row.digit_shift(1);
    }

    c.signbit = this->signbit * b.signbit;

    c.zero_justify();

    return c;
}


bignum bignum::operator / (bignum &b)
{
    bignum row = 0;                     /* represent shifted row */
    bignum tmp = 0;                     /* placeholder bignum */
    int asign, bsign;		/* temporary signs */
    int i,j;                        /* counters */

    bignum c = 0;

    c.signbit = this->signbit * b.signbit;

    asign = this->signbit;
    bsign = b.signbit;

    this->signbit = PLUS;
    b.signbit = PLUS;

    c.lastdigit = this->lastdigit;

    for (i=this->lastdigit; i>=0; i--) {
        row.digit_shift(1);
        row.digits[0] = this->digits[i];
        c.digits[i] = 0;
        while (row <= b) {
            c.digits[i] ++;
            tmp = row - b;
            row = tmp;
        }
    }

    c.zero_justify();

    this->signbit = asign;
    b.signbit = bsign;

    return c;
}

bignum bignum::operator ^ (int k)
{
    bignum s, t;

    s = 1;
    t = 1;
    while (k--) {
        t = *this * s;
        s = t;
    }

    return t;
}
