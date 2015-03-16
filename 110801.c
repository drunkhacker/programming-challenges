#include <stdio.h>
#include <string.h>

#define MAX_N 8
#define MAX_NN 64
#define abs(a) ((a)<0?(-(a)):(a))

int board_size;
int bishop_count;
long answer_count;

long white_count[MAX_N+1][MAX_NN+1];
long black_count[MAX_N+1][MAX_NN+1];

typedef struct cell_s cell_t;
struct cell_s {
    int row;
    int col;
};

void find_candidates(cell_t a[], int k, cell_t cand[], int *n_cand, int board)
{
    int r, c, i;
    //a[k]에 들어갈 수 있는 애들을 탐색한다
    //현재까지 a[k-1]까지의 답이 construct돼있음
    //
    //a[k-1]의 컬럼이 현재까지의 답 중에 가장 큰 컬럼
    *n_cand = 0;

    for (r = 0; r < board_size; r++) {
        for (c = (k == 0 ? 0 : a[k-1].col); c < board_size; c++) {
            if (board) { //black board
                if (r % 2 != c % 2)
                    continue;
            } else { //white board
                if (r % 2 == c % 2)
                    continue;
            }

            for (i = 0; i < k; i++) {
                if (a[i].col == c && a[i].row > r) {
                    break;
                }
                if (abs(a[i].row - r) == abs(a[i].col - c))
                    break;
            }

            if (i == k) {
                cand[*n_cand] = (cell_t){r, c};
                *n_cand = *n_cand + 1;
            }
        }
    }
}

void backtrack(cell_t a[], int k, int n, int board)
{
    int i;
    int c_cnt;
    cell_t c[MAX_NN];

    if (n == 0) {
        answer_count = 1;
        return;
    }

    if (k == n) {
        /* for (i = 0; i < k; i++) {
            printf("(%d,%d) ", a[i].row, a[i].col);
        }
        printf("\n"); */
        answer_count++;
    } else {
        find_candidates(a, k, c, &c_cnt, board);
        for (i = 0; i < c_cnt; i++) {
            a[k] = c[i];
            backtrack(a, k+1, n, board);
        }
    }
}

void process()
{
    int i, j;
    long c = 0;
    long c1 = 0;

    cell_t a1[MAX_NN];
    cell_t a2[MAX_NN];
    for (i = 0; i <= bishop_count; i++) {
        if (white_count[board_size][i] < 0) {
            answer_count = 0;
            backtrack(a1, 0, i, 0);
            white_count[board_size][i] = answer_count;
        }
        c1 = white_count[board_size][i];

        j = bishop_count - i;
        if (black_count[board_size][j] < 0) {
            answer_count = 0;
            backtrack(a2, 0, j, 1);
            black_count[board_size][j] = answer_count;
        }
        c1 *= black_count[board_size][j];

        c += c1;
    }

    printf("%ld\n", c);
}

int main()
{
    int i, n;

    for (n = 1; n <= MAX_N; n++) {
        for (i = 0; i <= MAX_NN; i++) {
            white_count[n][i] = -1;
            black_count[n][i] = -1;
        }
    }

    while (1) {
        scanf("%d%d", &board_size, &bishop_count);
        if (board_size == 0 && bishop_count == 0)
            break;

        process();
    }

    return 0;
}
