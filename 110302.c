#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_N 50

int find_word(char grid[MAX_N+2][MAX_N+2], int row, int col, char word[], int len);
int find_word_2(char grid[MAX_N+2][MAX_N+2], int row, int col, char word[], int len, int dr, int dc);

int main() {
  int t; //test cases
  int i,j; //loop var
  int m,n; //m = rows, n = cols
  int k; //number of words to find
  char c;
  char grid[MAX_N+2][MAX_N+2]; //char grid
  char word[MAX_N+1]; //word to find
  int row, col; //start row,col for finding word
  int find; // find flag

  scanf("%d", &t);
  /*printf("t = %d\n", t);*/
  getchar(); //dummy enter
  for (i=0; i<t; i++) {
    //input grid
    scanf("%d %d", &m, &n);
    /*printf("m = %d, n = %d\n", m, n);*/
    for (row=1; row<=m; row++) {
      for (col=1; col<=n;) {
        c = getchar();
        if (c != '\n') {
          if (isupper(c))
            c = tolower(c);

          grid[row][col] = c;
          col++;
        }
      }
    }
    //set border for grid
    for (col=0; col<=n+1; col++) {
      grid[0][col] = '-';
      grid[m+1][col] = '-';
    }
    for (row=0; row<=m+1; row++) {
      grid[row][0] = '-';
      grid[row][n+1] = '-';
    }

    //debug print grid
    /*for (i=0; i<=m+1; i++) {*/
      /*for (j=0; j<=n+1; j++) {*/
        /*printf("%c", grid[i][j]);*/
      /*}*/
      /*printf("\n");*/
    /*}*/

    //input words
    scanf("%d", &k);
    /*printf("k = %d\n", k);*/
    while (k--) {
      scanf("%s", word);
      //convert each char to lowercase
      for (j=0; j<strlen(word); j++)
        if (isupper(word[j]))
          word[j] = tolower(word[j]);


      find = 0;
      //find word in the grid
      for (row=1; row<=m; row++) {
        for (col=1; col<=n; col++) {
          if (find_word(grid, row, col, word, strlen(word))) {
            printf("%d %d\n", row, col);
            find = 1;
            break;
          }
        }
        if (find)
          break;
      }
    }
    if (i < t-1)
      printf("\n"); //for each test cases, there's a new line
  }

  return 0;
}

int find_word(char grid[MAX_N+2][MAX_N+2], int row, int col, char word[], int len) {
  int dr, dc; //direction of row, col finding

  for (dr=-1; dr<=1; dr++) {
    for (dc=-1; dc<=1; dc++) {
      if (dc == 0 && dr == 0) continue;

      if (find_word_2(grid, row, col, word, len, dr, dc))
        return 1;
    }
  }

  return 0;
}

int find_word_2(char grid[MAX_N+2][MAX_N+2], int row, int col, char word[], int len, int dr, int dc) {
  //search for one direction only
  int i = 0;
  int r = row; 
  int c = col;
  for (i=0; i<len; i++) {
    if (word[i] != grid[r][c]) return 0;
    r += dr;
    c += dc;
  }
  return 1;
}
