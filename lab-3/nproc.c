#include <stdio.h>
#include <math.h>

int main()
{
    float n = 28000;
    printf("%d", (int)(8 * n * n / (pow(2, 30) - 2 * n)) + 1);

    return 0;
}
