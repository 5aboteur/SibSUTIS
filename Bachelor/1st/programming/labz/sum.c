#include <stdio.h>
#include <math.h>
int main()
{
	int n, sum = 0, t, i = 0;
	printf("Kol-vo shagov: ");
	scanf ("%d", &n);
	while ( i < n )
		{
		printf("Input t: ");
		scanf ("%d", &t);
		sum=sum+t;
		i++;
		}
	printf("Summa = %d", sum);
	return 0;
}
