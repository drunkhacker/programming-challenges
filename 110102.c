#include <stdio.h>
#include <string.h>

#define MAX_N 102

void make_field(char field[][MAX_N], int rows, int cols);

int main() {
  int n,m; //mine field dimension row=n, col=m

  int i,j; //loop var
  int t=1; //case number
  char buf[BUFSIZ]; //buffer for line input
  char *pch; //pointer for iterating buf char by char.

  char field[MAX_N][MAX_N];
  while(1) {
    fgets(buf, BUFSIZ, stdin);
    sscanf(buf, "%d %d", &n, &m);
    if (n == 0 && m == 0) break;

    //init field
    for (i=0; i<=n+1; i++) 
      for (j=0; j<=m+1; j++)
        field[i][j] = '.';

    //input
    for (i=1; i<=n; i++) {
      fgets(buf, BUFSIZ, stdin);
      buf[strlen(buf)-1] = 0;
      pch = buf;
      /*printf("buf = %s", buf);*/

      for (j=1; j<=m; j++)
        field[i][j] = *pch++;
    }

    make_field(field, n, m);

    //print result
    if (t > 1) printf("\n");
    printf("Field #%d:\n", t++);
    for (i=1; i<=n; i++) {
      for (j=1; j<=m; j++) {
        printf("%c", field[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}

void make_field(char field[][MAX_N], int rows, int cols) {
  int i,j; //loop var. i for row, j for column
  int n; //mine number
  int r,c; //loop var for checking around 8 squares
  for (i=1; i<=rows; i++) {
    for (j=1; j<=cols; j++) {
      if (field[i][j] == '.') {
        n = 0;
        //check around
        for (r=i-1; r<=i+1; r++) {
          for (c=j-1; c<=j+1; c++) {
            if (r == i && c == j) continue; //is itself.
            n += (field[r][c] == '*');
          }
        }
        field[i][j] = '0' + n;
      }
    }
  }
}
