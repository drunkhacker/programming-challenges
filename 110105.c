#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 250
#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))

typedef struct point {
  int row;
  int col;
} Point;

/* point queue implementation */
typedef struct queue {
  int head;
  int tail;
  int capacity;
  Point *elems;
} Queue;

int queue_init(Queue *q, int capacity) {
  q->head = 0; 
  q->tail = 0;
  q->capacity = capacity + 1;
  q->elems = (Point *)calloc(q->capacity, sizeof(Point));
}

int queue_free(Queue *q) {
  free(q->elems);
}

int queue_is_empty(Queue *q) {
  return q->head == q->tail;
}

int queue_is_full(Queue *q) {
  return (q->tail+1) % q->capacity == q->head;
}

void queue_add_item(Queue *q, Point item) {
  q->elems[q->tail] = item;

  q->tail = (q->tail+1) % q->capacity;

  if (q->tail == q->head)
    q->head = (q->head+1) % q->capacity;
}

Point queue_remove_item(Queue *q) {
  Point e = q->elems[q->head]; 
  q->head = (q->head+1) % q->capacity;

  return e;
}
/* end of point queue implementation */

void fill_white(char field[][MAX_N+2], int rows, int cols) {
  //fill the board with white color = O
  int i,j;
  for (i=1; i<=rows; i++)
    for (j=1; j<=cols; j++)
      field[i][j] = 'O';
}

void flood_fill(char field[][MAX_N+2], int rows, int cols, int r, int c, char target, char replace);

int main() {

  char buf[BUFSIZ];
  char field[MAX_N+2][MAX_N+2];
  int rows, cols; //dimension of board
  int row, col; //coordinate of point
  char c; //color of point
  int i,j;
  int end = 0; //session end flag
  while(!end) {
    fgets(buf, BUFSIZ-1, stdin);
    /*printf("cmd : %s", buf);*/
    switch (buf[0]) {
      case 'I': 
        { //initialize board
          sscanf(buf + 1, "%d %d", &cols, &rows);
          //border process
          for (j=0; j<=cols+1; j++)
            field[0][j] = field[rows+1][j] = '-';
          for (i=0; i<=rows+1; i++)
            field[i][0] = field[i][cols+1] = '-';
          fill_white(field, rows, cols);
          break;
        }
      case 'C':
        fill_white(field, rows, cols);
        break;
      case 'L':
        { //set the pixel with color
          sscanf(buf + 1, "%d %d %c", &col, &row, &c);
          field[MAX(MIN(row, rows), 1)][MAX(MIN(col, cols), 1)] = c;
          break;
        }
      case 'V':
        { //V X Y1 Y2 C 
          //draw line from (Y1, X) to (Y2, X) with color c
          int y1, y2, x, t;
          sscanf(buf + 1, "%d %d %d %c", &x, &y1, &y2, &c);
          if (y1 > y2) { t = y1; y1 = y2; y2 = t; }
          y1 = MAX(MIN(y1, rows), 1);
          y2 = MAX(MIN(y2, rows), 1);
          x = MAX(MIN(x, cols), 1);

          for (row=y1; row<=y2; row++)
            field[row][x] = c;

          break;
        }
      case 'H': 
        { // H X1 X2 Y C
          //draw line from (Y, X1) to (Y, X2) with color c
          int x1, x2, y, t;
          sscanf(buf + 1, "%d %d %d %c", &x1, &x2, &y, &c);
          if (x1 > x2) { t = x1; x1 = x2; x2 = t; }
          x1 = MAX(MIN(x1, cols), 1);
          x2 = MAX(MIN(x2, cols), 1);
          y = MAX(MIN(y, rows), 1);

          for (col=x1; col<=x2; col++)
            field[y][col] = c;

          break;
        }
      case 'K':
        { // K X1 Y1 X2 Y2 C
          //draw rectangle fill with C. (Y1, X1) left top, (Y2, X2) right bottom
          int x1, x2, y1, y2, t;
          sscanf(buf + 1, "%d %d %d %d %c", &x1, &y1, &x2, &y2, &c);
          y1 = MAX(MIN(y1, rows), 1);
          y2 = MAX(MIN(y2, rows), 1);
          x1 = MAX(MIN(x1, cols), 1);
          x2 = MAX(MIN(x2, cols), 1);

          if (y2 >= y1 && x2 >= x1) {
            for (row=y1; row<=y2; row++)
              for (col=x1; col<=x2; col++)
                field[row][col] = c;
          }

          break;
        }
      case 'F':
        { //F X Y C : flood fill from (y,x)
          sscanf(buf + 1, "%d %d %c", &col, &row, &c);
          row = MAX(MIN(row, rows), 1);
          col = MAX(MIN(col, cols), 1);
          flood_fill(field, rows, cols, row, col, field[row][col], c);
          break;
        }
      case 'S':
        { //S Name 
          // print name, and print image
          char name[BUFSIZ];
          sscanf(buf + 2, "%s", name);
          /*name[strlen(name) - 1] = 0;*/
          printf("%s\n", name);
          //print image
          for (row=1; row<=rows; row++) {
            for (col=1; col<=cols; col++)
              printf("%c", field[row][col]);
            printf("\n");
          }
          break;
        }
      case 'X':
        end = 1;
        break;
    }
  }


  return 0;
}

void flood_fill(char field[][MAX_N+2], int rows, int cols, int r, int c, char target, char replace) {
  /*printf("r, c = %d %d\n", r, c);*/
  Queue q;
  int w, e; //west and east bound of a specific row
  int i; //loop var

  if (field[r][c] != target) return;
  queue_init(&q, MAX_N*2);

  int check[MAX_N+2][MAX_N+2] = {0,};

  queue_add_item(&q, (Point){r, c});
  while(!queue_is_empty(&q)) {
    Point n = queue_remove_item(&q);

    if (field[n.row][n.col] == target) {
      w = n.col; 
      e = n.col;

      while(field[n.row][w] == target && w >= 1)
        w--;
      w++;
      while(field[n.row][e] == target && e <= cols) 
        e++;
      e--;

      /*printf("w = %d, e = %d\n", w, e);*/

      for (i=w; i<=e; i++) {
        field[n.row][i] = replace;
        check[n.row][i] = 1;

        if (field[n.row-1][i] == target && check[n.row-1][i] == 0) {
          /*printf("add point (%d, %d)\n", n.row-1, i);*/
          queue_add_item(&q, (Point){n.row-1, i});
        }
        if (field[n.row+1][i] == target && check[n.row-1][i] == 0) {
          /*printf("add point (%d, %d)\n", n.row+1, i);*/
          queue_add_item(&q, (Point){n.row+1, i});
        }
      }
    }
  }
  queue_free(&q);
}
