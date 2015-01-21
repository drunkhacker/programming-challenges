#include <stdio.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

int cycle_len(unsigned int n);

int main() {
  unsigned int i,j,n,t; //t is for swap
  int max_len, c_len; //max_len = maximum cycle length, c_len = cycle length
  while(scanf("%d %d", &i, &j) != EOF) {
    printf("%d %d ", i, j);
    if (i>j) { t = i; i = j; j = t; }
    max_len = 0;
    for (n=i; n<=j; n++) {
      c_len = cycle_len(n);
      max_len = MAX(max_len, c_len);
    }
    printf("%d\n", max_len);
  }

  return 0;
}

int cycle_len(unsigned int n) {
  //n을 받아서 싸이클 길이를 구함.
  int c = 1;
  while (n != 1) {
    if (n % 2 == 0) 
      n /= 2;
    else
      n = 3*n + 1;

    c++;
  }
  return c;
}
