#include <stdio.h>
#include <string.h>

int board[10][10] = {'.',};

void init()
{
    memset(board, '.', 100);
}

int input()
{
    int i, j;
    int empty = 1;
    for (i=1; i<=8; i++) {
        for (j=1; j<=8; j++) {
            board[j][i] = getc(stdin);
            empty = empty && board[j][i] == '.';
        }
        getc(stdin);
    }

    return !empty;
}

int go(int target, int fx, int fy, int dx, int dy)
{
    int x, y;
    /*printf("target=%c, fx=%d, fy=%d, dx=%d, dy=%d\n", target, fx, fy, dx, dy);*/

    for (x = fx + dx, y = fy + dy;
         1 <= x && x <= 8 && 1 <= y && y <= 8;
         x += dx, y += dy) {
        /*printf("board[%d][%d]=%c\n", x, y, board[x][y]);*/
        if (board[x][y] == '.') continue;
        return board[x][y] == target;
    }
    return 0;
}

int check(int p, int x, int y)
{
    int dx, dy;
    int target = 'k';
    int dir = -1;
    int rc;

    if (p > 'Z') {
        /* p is lower? p == black */
        target = 'K';
        dir = 1;

        p -= 32;
    }

    switch (p) {
        case 'P':
            rc = board[x+1][y+dir] == target || board[x-1][y+dir] == target;
            break;
        case 'R':
            /*printf("R\n");*/
            rc = 
                go(target, x, y,  1 , 0) ||
                go(target, x, y, -1,  0) ||
                go(target, x, y,  0,  1) ||
                go(target, x, y,  0, -1);
            break;
        case 'B':
            /*printf("B\n");*/
            rc =
                go(target, x, y,  1 , 1) ||
                go(target, x, y, -1, -1) ||
                go(target, x, y,  1, -1) ||
                go(target, x, y, -1,  1);
            break;

        case 'Q':
            /*printf("Q\n");*/
            rc = 0;
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    rc = go(target, x, y, dx, dy);
                    if (rc) goto out;
                }
            }
            break;

       case 'K':
            for (dx = -1; dx <= 1; dx++) {
                for (dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    rc = board[x+dx][y+dy] == target;
                    if (rc) goto out;
                }
            }
            break;

       case 'N':
            // (-2, -1) (-2, 1)
            // (-1, -2) (-1, 2)
            // ( 1, -2) ( 1, 2)
            // ( 2, -1) ( 2, 1)
            for (dx = -2; dx <= 2; dx++) {
                if (dx == 0) continue;
                dy = 3 - (dx < 0 ? -dx : dx);
                rc = board[x+dx][y+dy] == target ||
                     board[x+dx][y-dy] == target;
                if (rc) goto out;
            }
            break;
    }
out:
    return dir*rc;
}

int process()
{
    int i, j;
    int rc = 0;

    for (i = 1; i <= 8; i++) {
        for (j = 1; j <= 8; j++) {
            if (board[i][j] != '.' && (rc = check(board[i][j], i, j)) != 0)
                return rc;
        }
    }

    return rc;
}

int main()
{
    int c = 1;
    int r = 0;

    init();
    while(1) {
        if (!input())
            break;
        r = process();
        printf("Game #%d: %s king is in check.\n", c++, r < 0 ? "black" : r > 0 ? "white" : "no");
        getc(stdin);
    }
    return 0;
}

