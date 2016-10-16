#include <stdio.h>
#include <math.h>
int main()
{
	int n, pr = 1, t, i = 0, z = 1;
	printf("Kol-vo shagov: ");
	scanf ("%d", &n);
	while ( i < n )
		{
		printf("Input t: ");
		scanf ("%d", &t);
		pr=pr*t;
		i++;
		}
	printf("poluchilos' = %d", pr);
	return 0;
}
