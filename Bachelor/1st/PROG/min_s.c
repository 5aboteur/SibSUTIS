#include <stdio.h>
#include <math.h>
int main()
{
	int n, max, t, i = 0;
	printf("Input n: ");
	scanf ("%d", &n);
	printf("Input max: ");
	scanf ("%d", &max);
	while ( i < (n-1) )
		{
		printf("Imput t: ");
		scanf ("%d", &t);
		if(t>max)
			{
			max=t;
			}
		i++;
		}
	printf("max = %d", max);
	return 0;
}
