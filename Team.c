#include <stdio.h>
#include <stdlib.h>

int main()
{

    int nbr;
    int i = 0;
    int j = 0;
    int counter = 0;
    int n_count;

    scanf("%d", &nbr);
    int arr[nbr][3];
    i = 0;
    while(i < nbr)
    {
        j = 0;
        while (j < nbr)
        scanf("%d", &arr[i][j++]);
        i++;
    }
    for (i = 0; i < nbr; i++)
    {
        n_count = 0;
        for (j = 0; j < 3; j++)
            if (arr[i][j] == 1)
            n_count++;
        if (n_count >= 2)
            counter++;
    }
    printf("%d\n", counter);
}
 
