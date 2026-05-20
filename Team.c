#include <stdio.h>
#include <string.h>

int main()
{
    char x[4];
    int n;
    int counter = 0;

    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%3s", x);
        if (!strcmp("X++", x) || !strcmp("++X", x))
            counter++;
        else if (!strcmp("--X", x) || !strcmp("X--", x))
            counter--;
        else
            return 1;
    }
    printf("%d\n", counter);
    return 0;
}