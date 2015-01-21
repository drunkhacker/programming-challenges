#include <stdio.h>

#define MAX_N 1000
#define MAX(a, b) (((a)>(b))?(a):(b))

double calc_transfer(double c[], int n);

int main() {
  int n; //number of friends
  int i; //loop var

  double c[MAX_N]; //c[i] = amount of money spent by friend i

  double t; //minimum amount of transfer

  while(1) {
    scanf("%d", &n);
    if (n == 0) break;

    //input phase
    for (i=0; i<n; i++) {
      scanf("%lf", &c[i]);
    }

    t = calc_transfer(c, n);
    printf("$%.2lf\n", t);
  }

  return 0;
}

double calc_transfer(double c[], int n) {
  int i; //loop var
  double avg = 0; //average of spent money
  double s = 0; //sum of positive diffs
  double s2 = 0; //sum of negative diffs
  double diff;

  for (i=0; i<n; i++)
    avg += c[i];

  avg = ((long)(avg*100) / n)/100.0;

  for (i=0; i<n; i++) {
    diff = avg - c[i];
    if (diff < 0)
      s2 += diff;
    else
      s += diff;
  }

  /*printf("positive : %d, negative : %d\n", s, s2);*/

  return MAX(-s2, s);
}

