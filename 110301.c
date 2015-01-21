#include <stdio.h>
#include <string.h>

int main() {
  char buf[BUFSIZ]; //input buffer
  char map[128+1] = {0,};

  int i,j; //loop var
  char c;

  //initialize map
  char *lines[] = {"`1234567890-=","QWERTYUIOP[]\\","ASDFGHJKL;'","ZXCVBNM,./"};
  for (i=0; i<4; i++) {
    for (j=1; j<strlen(lines[i]); j++) 
      map[lines[i][j]] = lines[i][j-1];
  }

  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n')
      putchar(c);
    else
      putchar(map[c]);
  }
  putchar('\n');

  return 0;
}
