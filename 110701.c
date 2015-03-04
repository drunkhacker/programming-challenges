#include <stdio.h>
#include <math.h>

int main() 
{
    unsigned int n;
    int rn;

    while (1) {
        scanf("%d", &n);

        if (n == 0)
            break;

        rn = sqrt(n);
        if (rn*rn == n) 
            printf("yes\n");
        else
            printf("no\n");
    }

    return 0;
}
