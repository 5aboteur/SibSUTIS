#include <stdio.h>
#include <math.h>
int main()
{
	int n, sum = 0, a, i = 0, z=1, j=0, x;
	printf("Kol-vo shagov: ");
	scanf ("%d", &n);
	scanf ("%d", &x); 
	while ( i < n )
		{
		while (j<i)
		 	{
			z=z*x;
			j++;
			}
		scanf ("%d", &a);
		sum=sum+a*z;
		i++;
		}
	printf("Summa = %d", sum);
	return 0;
}
