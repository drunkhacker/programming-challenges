#include <stdio.h>

#define MAX_N 3650

int days[MAX_N+1];

int main() {
  int t; //test cases
  int n; //simulation days
  int p; //number of parties

  int i,j,k;
  int h; //hartal factor
  int c; //hartal days count
  scanf("%d", &t);

  for (i=0; i<t; i++) {
    scanf("%d", &n);

    //init.
    for (j=1; j<=n; j++)
      days[j] = 0;
    scanf("%d", &p);
    c = 0;

    for (j=0; j<p; j++) {
      scanf("%d", &h);

      //simulation
      for (k=h; k<=n; k+=h) {
        //check friday or saturday
        if (k % 7 == 0 || k % 7 == 6)
          continue;

        if (!days[k]) {
          days[k] = 1;
          c++;
        }
      }
    }
    printf("%d\n", c);
  }

  return 0;
}
