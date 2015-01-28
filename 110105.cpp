#include <stdio.h>
#include <utility>
#include <queue>
#include <string.h>

#define MIN(a,b) ((a)>(b)?(b):(a))
#define MAX(a,b) ((a)<(b)?(b):(a))

char image[255][255];
char buf[BUFSIZ];
char name[13];
char cmd;

using namespace std;

struct point {
    int x;
    int y;
};
typedef struct point point;

int x, y, x1, y1, x2, y2;
char color;

int rows, cols; /*canvas size*/

int input()
{
    fgets(buf, BUFSIZ - 1, stdin);
    if (buf[0] == 'X')
        return 0;

    cmd = buf[0];
    switch (cmd) {
        case 'I':
            sscanf(buf+2, "%d %d",  &cols, &rows);
            break;
        case 'C':
            break;
        case 'L':
            sscanf(buf+2, "%d %d %c", &x, &y, &color);
            break;
        case 'V':
            sscanf(buf+2, "%d %d %d %c", &x, &y1, &y2, &color);
            break;
        case 'H':
            sscanf(buf+2, "%d %d %d %c", &x1, &x2, &y, &color);
            break;
        case 'K':
            sscanf(buf+2, "%d %d %d %d %c", &x1, &y1, &x2, &y2, &color);
            break;
        case 'F':
            sscanf(buf+2, "%d %d %c", &x, &y, &color);
            break;
        case 'S':
            sscanf(buf+2, "%s", name);
            break;
        default:
            break;
    }

    return 1;
}

void clear()
{
    int r, c;
    for (r = 1; r <= rows; r++) {
        for (c = 1; c <= cols; c++) {
            image[r][c] = 'O';
        }
    }
}

void init()
{
    memset(image, 0, sizeof(image));
    clear();
}

void dot()
{
    image[y][x] = color;
}

void vline()
{
    int r;
    for (r = MIN(y1, y2); r <= MAX(y1, y2); r++) {
        image[r][x] = color;
    }
}

void hline()
{
    int c;
    for (c = MIN(x1, x2); c <= MAX(x1, x2); c++) {
        image[y][c] = color;
    }
}

void rect()
{
    int x, y;

    /* (x1, y1) : upper left
       (x2, y2) : lower right */
    if (!(x2 >= x1 && y2 >= y1)) return;

    for (y = y1; y <= y2; y++) {
        for (x = x1; x <= x2; x++) {
            image[y][x] = color;
        }
    }
}

void fill()
{
    queue<point> q;
    point w, e;
    int _x;
    char target_color;

    q.push((point){x,y});
    target_color = image[y][x];

    if (target_color == color) return;

    while (!q.empty()) {
        point p = q.front();
        q.pop();
        w = p;
        e = p;

        while(image[w.y][w.x - 1] == target_color && w.x - 1 >= 0) w.x--;
        while(image[e.y][e.x + 1] == target_color && e.x + 1 <= cols + 1) e.x++;

        for (_x = w.x; _x <= e.x; _x++) {
            // printf("(%d,%d) = %c\n", _x, p.y, color);
            image[p.y][_x] = color;

            if (image[p.y - 1][_x] == target_color)
                q.push((point){_x, p.y - 1});
            if (image[p.y + 1][_x] == target_color)
                q.push((point){_x, p.y + 1});
        }
    }
}


int process()
{
    switch (cmd) {
        case 'I':
            init(); return 0;
        case 'C':
            clear(); return 0;
        case 'L':
            dot(); return 0;
        case 'V':
            vline(); return 0;
        case 'H':
            hline(); return 0;
        case 'K':
            rect(); return 0;
        case 'F':
            fill(); return 0;
        case 'S':
            return 1;
    }
    return 0;
}

void output()
{
    int r, c;
    printf("%s\n", name);
    for (r = 1; r <= rows; r++) {
        for (c = 1; c <= cols; c++) {
            fputc(image[r][c], stdout);
        }
        fputc('\n', stdout);
    }
}

int main()
{
    while (1) {
        if (!input()) 
            break;
        if (process())
            output();
    }
    return 0;
}
