#include <stdio.h>
#include <string.h>

int white_board[8][8]; //board[i][j]>0 <=> position(i,j) is placed or not avail.
int black_board[8][8];

long white_count[64];
long black_count[64];

typedef struct cell_s cell_t;
struct cell_s {
    int row;
    int col;
};

cell_t empty_cells[64];


void place2(int row, int col, int board[8][8], int board_size, int d)
{
    int dr, dc;
    int r, c;

    board[row][col] += d;
    /* printf("place2\n"); */
    /* printf("(r,c) = (%d,%d)\n", row, col); */

    for (dr = -1; dr <= 1; dr += 2) {
        for (dc = -1; dc <= 1; dc += 2) {
            for (r = row, c = col; 
                 r >= 0 && r < board_size && c >= 0 && c < board_size;
                 r += dr, c += dc) {
                if (r == row && c == col)
                    continue;

                /* printf("(r,c) = (%d,%d)\n", r, c); */
                board[r][c] += d;
            }
        }
    }
}

void place(int row, int col, int board[8][8], int board_size)
{
    place2(row, col, board, board_size, 1);
}

void displace(int row, int col, int board[8][8], int board_size)
{
    place2(row, col, board, board_size, -1);
}

int get_empty_cell(int board[8][8], int board_size, cell_t *result)
{
    int r, c;
    int n = 0;
    for (r = 0; r < board_size; r++) {
        for (c = 0; c < board_size; c++) {
            if (board[r][c] == 0)
                result[n++] = (cell_t){r,c};
        }
    }

    return n;
}

long backtrack(int i, int k, int board[8][8], int board_size)
{
    long c = 0;
    int empty_cnt;
    int j;
    cell_t cell;

    if (k == 0) 
        return 0;

    if (i == k)
        return 1;

    empty_cnt = get_empty_cell(board, board_size, empty_cells);

    if (empty_cnt < k - i)
        return 0;

    for (j = 0; j < empty_cnt; j++) {
        cell = empty_cells[j];
        place(cell.row, cell.col, board, board_size);
        c += backtrack(i + 1, k, board, board_size);
        displace(cell.row, cell.col, board, board_size);
        if (empty_cnt != get_empty_cell(board, board_size, empty_cells)) {
            printf("error! empty_cnt=%d, empty_cnt2=%d\n", empty_cnt,
                    get_empty_cell(board, board_size, empty_cells));
            exit(1);

        }
    }

    return c;
}

long process(int n, int k)
{
    int i, j, m;
    long c = 0;
    long c1, c2;

    for (m = 0; m <= k; m++) {
        /* memset(white_count, 0, sizeof(white_count)); */
        /* memset(black_count, 0, sizeof(black_count)); */

        memset(white_board, 0, sizeof(white_board));
        memset(black_board, 0, sizeof(black_board));

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                if (i % 2 == j % 2) {
                    white_board[i][j] = 1;
                    /* printf("* "); */
                    /* printf("(r,c)=(%d,%d)\n", i, j); */
                } else {
                    black_board[i][j] = 1;
                    /* printf("  "); */
                }
            }
                /* printf("\n"); */
        }

        c1 = backtrack(0, m, white_board, n);
        c2 = backtrack(0, k-m, black_board, n);

        printf("white_count(%d, %d) = %ld, black_count(%d, %d) = %ld\n",
                n, m, c1, n, k-m, c2);

        c += c1 + c2;
    }

    return c;
}

int main()
{
    int n, k;

    while (1) {
        scanf("%d%d", &n, &k);
        if (n == 0 && k == 0)
            break;

        printf("%ld\n", process(n, k));
    }

    return 0;
}
